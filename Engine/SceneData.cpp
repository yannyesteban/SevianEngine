#include "SceneData.h"


void decomposeTransform ( const glm::mat4& transform, glm::vec3& position, glm::quat& rotation, glm::vec3& scale ) {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose ( transform, scale, rotation, position, skew, perspective );
}

namespace SEVIAN {
    void ProcessSkeletonHierarchy ( aiNode* node, SkeletonComponent& skeleton, int parentIndex = -1 ) {
        // Busca si este nodo corresponde a un hueso
        auto boneIt = skeleton.boneMapping.find ( node->mName.C_Str () );
        if (boneIt != skeleton.boneMapping.end ()) {
            int currentBoneIndex = boneIt->second;
            skeleton.bones[currentBoneIndex].parentIndex = parentIndex;
        }

        // Procesa los hijos de este nodo
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessSkeletonHierarchy ( node->mChildren[i], skeleton, boneIt != skeleton.boneMapping.end () ? boneIt->second : -1 );
        }
    }
    void processNodeHierarchy ( const aiNode* node, const std::unordered_map<std::string, int>& boneMap,
        std::vector<Bone>& bones, int parentIndex = -1 ) {
        std::string nodeName = node->mName.C_Str ();

        if (boneMap.find ( nodeName ) != boneMap.end ()) {
            int boneIndex = boneMap.at ( nodeName );
            bones[boneIndex].localTransform = glm::transpose ( glm::make_mat4 ( &node->mTransformation.a1 ) );
            bones[boneIndex].parentIndex = parentIndex;
        }

        // Recurre para los hijos
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNodeHierarchy ( node->mChildren[i], boneMap, bones, boneMap.find ( nodeName ) != boneMap.end () ? boneMap.at ( nodeName ) : parentIndex );
        }
    }

    std::vector<Animation> loadAnimations ( const aiScene* scene ) {
        std::vector<Animation> animations;

        for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
            aiAnimation* anim = scene->mAnimations[i];
            Animation animation;
            animation.name = anim->mName.C_Str ();
            animation.duration = anim->mDuration;
            animation.ticksPerSecond = anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 25.0f;

            for (unsigned int j = 0; j < anim->mNumChannels; j++) {
                aiNodeAnim* channel = anim->mChannels[j];
                AnimationChannel animChannel;
                animChannel.nodeName = channel->mNodeName.C_Str ();

                // Cargar claves de posición
                for (unsigned int k = 0; k < channel->mNumPositionKeys; k++) {
                    KeyFrame keyframe;
                    keyframe.time = channel->mPositionKeys[k].mTime;
                    keyframe.position = glm::vec3 (
                        channel->mPositionKeys[k].mValue.x,
                        channel->mPositionKeys[k].mValue.y,
                        channel->mPositionKeys[k].mValue.z
                    );
                    animChannel.positionKeys.push_back ( keyframe );
                }

                // Cargar claves de rotación
                for (unsigned int k = 0; k < channel->mNumRotationKeys; k++) {
                    KeyFrame keyframe;
                    keyframe.time = channel->mRotationKeys[k].mTime;
                    keyframe.rotation = glm::quat (
                        channel->mRotationKeys[k].mValue.w,
                        channel->mRotationKeys[k].mValue.x,
                        channel->mRotationKeys[k].mValue.y,
                        channel->mRotationKeys[k].mValue.z
                    );
                    animChannel.rotationKeys.push_back ( keyframe );
                }

                // Cargar claves de escala
                for (unsigned int k = 0; k < channel->mNumScalingKeys; k++) {
                    KeyFrame keyframe;
                    keyframe.time = channel->mScalingKeys[k].mTime;
                    keyframe.scale = glm::vec3 (
                        channel->mScalingKeys[k].mValue.x,
                        channel->mScalingKeys[k].mValue.y,
                        channel->mScalingKeys[k].mValue.z
                    );
                    animChannel.scaleKeys.push_back ( keyframe );
                }

                animation.channels.push_back ( animChannel );
            }

            animations.push_back ( animation );
        }

        return animations;
    }


    std::shared_ptr<SEVIAN::Entity> addEntity ( const aiScene* scene1, Scene& scene, aiMesh* mesh ) {

        auto entity = scene.createEntity ();


        std::vector<Vertex> vertices = { };
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D vertex1 = mesh->mVertices[i];
            aiVector3D normal1 = mesh->mNormals[i];
            //aiVector3D texCoord1 = mesh->mTextureCoords[0][i];
            Vertex vertex {};

            vertex.position = { vertex1.x, vertex1.y, vertex1.z };
            vertex.normal = { normal1.x, normal1.y, normal1.z };
            //vertex.texCoords = { texCoord1.x, texCoord1.y };
            vertex.color = { 0.05f, 0.58f, 0.6f };
            if (mesh->mTextureCoords[0]) {
                aiVector3D texCoord1 = mesh->mTextureCoords[0][i];
                vertex.texCoords = { texCoord1.x, texCoord1.y };
            }
            else {
                vertex.texCoords = { 0.0f, 0.0f }; // Coordenadas UV predeterminadas
            }
            //vertex.texCoords = { vertex1.}
            //std::cout << " Vertex " << vertex1.x;
            vertices.push_back ( vertex );
            // Utiliza los componentes x, y, z de vertex para tus operaciones
        }

        std::vector<uint32_t> indices;

        // Recorre cada cara de la malla
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];

            // Cada cara debe tener exactamente 3 índices (triangulada previamente)
            if (face.mNumIndices != 3) {
                std::cerr << "Non-triangle face detected. Skipping.\n";
                continue;
            }

            // Agrega los índices de la cara al vector
            indices.push_back ( face.mIndices[0] );
            indices.push_back ( face.mIndices[1] );
            indices.push_back ( face.mIndices[2] );
        }


        // Add components to the entity
       
        entity->addComponent<TransformComponent> ();

        // Procesar huesos
        if (mesh->HasBones ()) {
            SkeletonComponent skeletonComponent;
            int boneCount = 0;
            int boneCounter = 0; // Total de huesos
            for (unsigned int i = 0; i < mesh->mNumBones; i++) {
                aiBone* bone = mesh->mBones[i];

                // Agregar el hueso al mapping si no existe
                if (skeletonComponent.boneMapping.find ( bone->mName.C_Str () ) == skeletonComponent.boneMapping.end ()) {
                    
                    skeletonComponent.boneMapping[bone->mName.C_Str ()] = boneCount++;

                    Bone newBone;
                    newBone.name = bone->mName.C_Str ();
                    newBone.offsetMatrix = glm::transpose ( glm::make_mat4 ( &bone->mOffsetMatrix.a1 ) );
                    skeletonComponent.bones.push_back ( newBone );
                }

                // Asociar huesos a vértices
                int boneID = boneCounter++;


                for (unsigned int j = 0; j < bone->mNumWeights; j++) {
                    unsigned int vertexID = bone->mWeights[j].mVertexId;
                    float weight = bone->mWeights[j].mWeight;

                    for (int k = 0; k < 4; k++) {
                        if (vertices[vertexID].boneWeights[k] == 0.0f) {
                            vertices[vertexID].boneIDs[k] = boneID;
                            vertices[vertexID].boneWeights[k] = weight;
                            break;
                        }
                    }
                    //vertices[vertexID].weights.
                    // Asignar IDs de huesos y pesos a los vértices
                   // vertices[vertexID].addBoneWeight ( skeletonComponent.boneMapping[bone->mName.C_Str ()], weight );
                }


            }
            ProcessSkeletonHierarchy ( scene1->mRootNode, skeletonComponent );
            entity->addComponent<MeshComponent> ( vertices, indices );
            // Agregar transformaciones iniciales
            skeletonComponent.boneTransforms.resize ( boneCount, glm::mat4 ( 1.0f ) );

            // Agregar componente esqueleto a la entidad
            entity->addComponent<SkeletonComponent> ( skeletonComponent );
        }

       


        TextureComponent texture { "room", "textures/viking_room.png" };
        entity->addComponent<TextureComponent> ( texture );

        MaterialComponent material = MaterialComponent (
            { 0.1f, 0.1f, 0.1f, 1.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 0.5f, 0.5f, 0.5f, 1.0f },
            32.0f
        );
        entity->addComponent<MaterialComponent> ( material );
        float deltaX = 0.0, deltaY = 0.0, deltaZ = 0.0;
        auto name = Key::N4;
        //entity->addComponent<PositionComponent> ( glm::vec3 ( deltaX, deltaY, deltaZ ) );
        //entity->addComponent<RotationComponent> ( glm::vec3 ( 0.0f, 0.0f, 0.0f ) );
        //entity->addComponent<ScaleComponent> ( glm::vec3 ( 1.0f, 1.0f, 1.0f ) );
        entity->addComponent<InputComponent> ();

        entity->addComponent<NameComponent> ( name );

        return entity;
    }

    void createActorEntity ( Scene& scene, const std::string& file ) {



       // auto entity = scene.createEntity ();


        const struct aiScene* scene1 = aiImportFile ( file.c_str (),
            //aiProcess_CalcTangentSpace |
            //aiProcess_Triangulate |
            //aiProcess_JoinIdenticalVertices |
            //aiProcess_SortByPType 
            // 
            // 
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate |
            aiProcess_FlipUVs
        );

        // If the import failed, report it
        if (NULL == scene1) {
            std::cout << (aiGetErrorString ());
            return;
        }

        auto animations = loadAnimations ( scene1 );

        auto parentTransform = glm::mat4 ( 1.0 );

        glm::mat4 localTransform = glm::transpose ( glm::make_mat4 ( &scene1->mRootNode->mTransformation.a1 ) );
        glm::vec3 position, scale;
        glm::quat rotation;


        if (scene1->HasAnimations ()) {
            for (unsigned int i = 0; i < scene1->mNumAnimations; ++i) {
                aiAnimation* anim = scene1->mAnimations[i];
                std::cout << "Animación: " << anim->mName.C_Str () << "\n";
                std::cout << "Duración: " << anim->mDuration << " ticks\n";
                std::cout << "Ticks por segundo: " << anim->mTicksPerSecond << "\n";

                for (unsigned int j = 0; j < anim->mNumChannels; ++j) {
                    aiNodeAnim* channel = anim->mChannels[j];
                    std::cout << "Canal para nodo: " << channel->mNodeName.C_Str () << "\n";
                    std::cout << "Claves de posición: " << channel->mNumPositionKeys << "\n";
                    std::cout << "Claves de rotación: " << channel->mNumRotationKeys << "\n";
                    std::cout << "Claves de escala: " << channel->mNumScalingKeys << "\n";
                }
            }
        }

        for (int k = 0; k < scene1->mRootNode->mNumChildren; k++) { 
        
            auto children = scene1->mRootNode->mChildren[k];

            auto kk = children->mNumMeshes;
            


            glm::mat4 localTransform = glm::transpose ( glm::make_mat4 ( &children->mTransformation.a1 ) );
            glm::vec3 position, scale;
            glm::quat rotation;
            decomposeTransform ( localTransform, position, rotation, scale );
            std::cout << "test ";

            //aiMesh* mesh = scene1->mMeshes[k];


            for (int j = 0; j < children->mNumMeshes; j++) {

                unsigned int meshIndex = children->mMeshes[j];
                if (meshIndex >= scene1->mNumMeshes) {
                    std::cerr << "Invalid mesh index: " << meshIndex << std::endl;
                    continue;
                }


                aiMesh* mesh = scene1->mMeshes[children->mMeshes[j]];
                auto entity = addEntity ( scene1, scene, mesh );

                entity->addComponent<PositionComponent> ( position );
                entity->addComponent<RotationComponent> ( glm::eulerAngles ( rotation ) );
                entity->addComponent<ScaleComponent> ( scale );

                auto aniComponent = AnimationComponent {};
                aniComponent.animations = animations;
                aniComponent.animation = animations[0];
                entity->addComponent<AnimationComponent> ( aniComponent );
            }

           
        
        }
        decomposeTransform ( localTransform, position, rotation, scale );


        // Calcular la transformación global combinando con la del padre
        glm::mat4 globalTransform = parentTransform * localTransform;

        for (unsigned int i = 0; i < scene1->mNumMeshes; i++) {
            aiMesh* mesh = scene1->mMeshes[i];

            //auto entity = addEntity ( scene, mesh );
            //processMesh ( mesh, globalTransform );
        }

       
        
        aiReleaseImport ( scene1 );
        
    }

    void processBones ( aiMesh* mesh, Skeleton& skeleton, std::unordered_map<std::string, int>& boneMapping, int& boneCount ) {
        for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName = bone->mName.C_Str ();

            // Si el hueso no está en el mapa, asignar un ID único
            if (boneMapping.find ( boneName ) == boneMapping.end ()) {
                boneMapping[boneName] = boneCount++;
                Bone newBone;
                newBone.name = boneName;
                newBone.offsetMatrix = glm::transpose ( glm::make_mat4 ( &bone->mOffsetMatrix.a1 ) );
                skeleton.bones[boneName] = newBone;
            }

            // Asociar pesos del hueso a los vértices de la malla
            for (unsigned int j = 0; j < bone->mNumWeights; ++j) {
                unsigned int vertexId = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;

                // Asocia el ID del hueso y el peso al vértice correspondiente
                // (Asegúrate de tener estructuras para almacenar pesos de huesos por vértice)
               // vertices[vertexId].addBoneWeight ( boneMapping[boneName], weight );
            }
        }
    }

    

}

void SEVIAN::loadScene ( const std::string& file ) {

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile ( file.c_str(),
        //aiProcess_CalcTangentSpace |
        //aiProcess_Triangulate |
        //aiProcess_JoinIdenticalVertices |
        //aiProcess_SortByPType 
        // 
        // 
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate | 
        aiProcess_FlipUVs
        );

    // If the import failed, report it
    if (NULL == scene) {
        std::cout << ( aiGetErrorString () );
        return;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    // Acceso a los vértices de la malla

    std::vector<Vertex> vertices = { };
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D vertex1 = mesh->mVertices[i];
        aiVector3D normal1 = mesh->mNormals[i];
        //aiVector3D texCoord1 = mesh->mTextureCoords[0][i];
        Vertex vertex {};
        
        vertex.position = { vertex1.x, vertex1.y, vertex1.z };
        vertex.normal = { normal1.x, normal1.y, normal1.z };
        //vertex.texCoords = { texCoord1.x, texCoord1.y };
        vertex.color = { 0.05f, 0.58f, 0.6f };
        if (mesh->mTextureCoords[0]) {
            aiVector3D texCoord1 = mesh->mTextureCoords[0][i];
            vertex.texCoords = { texCoord1.x, texCoord1.y };
        }
        else {
            vertex.texCoords = { 0.0f, 0.0f }; // Coordenadas UV predeterminadas
        }
        //vertex.texCoords = { vertex1.}
        //std::cout << " Vertex " << vertex1.x;
        vertices.push_back ( vertex );
        // Utiliza los componentes x, y, z de vertex para tus operaciones
    }

    std::vector<uint32_t> indices;

    // Recorre cada cara de la malla
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        // Cada cara debe tener exactamente 3 índices (triangulada previamente)
        if (face.mNumIndices != 3) {
            std::cerr << "Non-triangle face detected. Skipping.\n";
            continue;
        }

        // Agrega los índices de la cara al vector
        indices.push_back ( face.mIndices[0] );
        indices.push_back ( face.mIndices[1] );
        indices.push_back ( face.mIndices[2] );
    }


    // Now we can access the file's contents
    //DoTheSceneProcessing ( scene );

    // We're done. Release all resources associated with this import
    aiReleaseImport ( scene );
    return;
}


/*Animation anim;
anim.name = animation->mName.C_Str();
anim.duration = animation->mDuration;
anim.ticksPerSecond = animation->mTicksPerSecond != 0.0 ? animation->mTicksPerSecond : 25.0; // Valor predeterminado si está vacío

for (unsigned int j = 0; j < animation->mNumChannels; j++) {
    aiNodeAnim* channel = animation->mChannels[j];
    std::string boneName = channel->mNodeName.C_Str();

    BoneAnimation boneAnim;
    boneAnim.boneName = boneName;

    // Iterar por claves de posición
    for (unsigned int k = 0; k < channel->mNumPositionKeys; k++) {
        aiVector3D position = channel->mPositionKeys[k].mValue;
        double time = channel->mPositionKeys[k].mTime;

        KeyFrame keyFrame;
        keyFrame.time = time;
        keyFrame.position = glm::vec3(position.x, position.y, position.z);
        keyFrame.rotation = glm::quat(); // No hay rotación en este keyframe
        keyFrame.scale = glm::vec3(1.0f); // Escala por defecto

        boneAnim.keyFrames.push_back(keyFrame);
    }

    // Iterar por claves de rotación
    for (unsigned int k = 0; k < channel->mNumRotationKeys; k++) {
        aiQuaternion rotation = channel->mRotationKeys[k].mValue;
        double time = channel->mRotationKeys[k].mTime;

        KeyFrame keyFrame;
        keyFrame.time = time;
        keyFrame.position = glm::vec3(0.0f); // Posición por defecto
        keyFrame.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
        keyFrame.scale = glm::vec3(1.0f); // Escala por defecto

        boneAnim.keyFrames.push_back(keyFrame);
    }

    // Iterar por claves de escala
    for (unsigned int k = 0; k < channel->mNumScalingKeys; k++) {
        aiVector3D scale = channel->mScalingKeys[k].mValue;
        double time = channel->mScalingKeys[k].mTime;

        KeyFrame keyFrame;
        keyFrame.time = time;
        keyFrame.position = glm::vec3(0.0f); // Posición por defecto
        keyFrame.rotation = glm::quat(); // Rotación por defecto
        keyFrame.scale = glm::vec3(scale.x, scale.y, scale.z);

        boneAnim.keyFrames.push_back(keyFrame);
    }

    anim.boneAnimations.push_back(boneAnim);
}
*/
#pragma once
#include "TextEngine.h"

namespace SEVIAN {
    namespace TEXT {


        struct Fragment
        {
            Font font;
            std::string text;
            float scale;

        };

        struct Line
        {
            std::vector<Fragment> fragments;

        };

        class Box
        {
            //std::vector<Line> lines;
            std::vector<std::vector<Quad>> lines;
            float maxWidth;
            float maxHeight;
            float posX = 0.0f;
            float posY = 0.0f;
            uint32_t curretLine = 0;
            float lineHeight = 0.0f;
            float lastScale = 1.0f;

        public:
            Box ( float width, float height ) :maxWidth ( width ), maxHeight ( height ) { }
            void addText ( const std::string& text, Font font, float scale );
            std::vector<std::vector<Quad>> getLines ();
        };
        class Layout
        {
            std::vector<Box> boxes;
        };
    }


};




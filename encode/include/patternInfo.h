#ifndef _PATTERNINFO_H
#define _PATTERNINFO_H

struct pos {
int x, y;
};

class patternInfo {
    public:
        patternInfo();
        
        patternInfo(struct pos startpos, int width, int height) {
            m_start_pos = startpos;
            m_width = width;
            m_height = height;
        }
        
        void setStartpos(struct pos p) { m_start_pos.x = p.x; m_start_pos.y = p.y; }
        void setWidth(int width) { m_width = width; }
        void setHeight(int height) { m_height = height; }
        
        struct pos getStartpos() { return m_start_pos; }
        int getWidth() { return m_width; }
        int getHeight() { return m_height; }
        
    private:
        pos m_start_pos;
        int m_width;
        int m_height;       
};

#endif

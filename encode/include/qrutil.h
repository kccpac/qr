#ifndef _QRUTIL_H
#define _QRUTIL_H
//#include "global.h"
#include "qrconst.h"
#include "qrimage.h"
#include "qrparam.h"

enum ReturnAction {
    UNKNOWN_ACTION= -1,
    FINISH = 0, 
    NO_ACTION = 1,
    MOVE_ASIDE = 2,
    PASS_THROUGH = 4,
    CHANGE_DIR = 8
};

typedef struct ReturnValue {
    int idx;
    int jump_step;
    int action;
    pos cpos;
} ReturnValue;

class qrutil {
    public:
        qrutil();
        qrutil(qrimage *image, qrparam *parm, int cwsize, int winsize);
                void init(qrimage *image, qrparam *parm, int cwsize, int winsize);
        ~qrutil();
         int locateCW();
    private:
        int getPattern(pos ppos);
        bool isHorz(std::list<pos> posMap);
        void setSymbolBit(int *map, pos cpos, std::list<pos> posMap, int dir_sign);
        ReturnValue check_action(pos ppos, bool isHrzAction, bool isDiag);
        ReturnValue getNextAction(pos cpos, pos ppos[], PLACEMENT_TYPE sdir, bool isMapOdd);
        ReturnValue getPathIdx(int patterns, int i, bool isHorz);
    //            ReturnValue getNextAction(pos ppos[]);
        ReturnValue getNextPosition(pos cpos, PLACEMENT_TYPE sdir, int map_dim, int mapPitch);
        int m_swin_size; // search window size        
        int m_cword_size;
        qrparam *m_param;
        qrimage *m_qrimage;
};

#endif

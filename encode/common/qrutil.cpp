#include <list>

#include "stdio.h"
#include "memory.h"
//#include "qrconst.h"
//#include "qrimage.h"
//#include "qrparam.h"
#include "qrutil.h"
 
 qrutil::qrutil() {
 //       m_swin_size = 0;
 //       m_cword_size = 0;
 //       m_param = NULL;
 //       m_qrimage = NULL;
 }
 
qrutil::qrutil(qrimage *image, qrparam *parm, int cwsize, int winsize):qrutil() {
   init(image, parm, cwsize, winsize);
}
 
 qrutil::~qrutil() {
     m_param = NULL;
    m_qrimage = NULL;
 }
 
 void qrutil::init(qrimage *image, qrparam *parm, int cwsize, int winsize) {
 
        m_swin_size =winsize;
        m_cword_size = cwsize;
        m_param = parm;
        m_qrimage = image;
 }
 
int qrutil:: getPattern(pos ppos) {
 
    int *map = NULL;
    int pattern = QUIET_ZONE;
//    ReturnValue ret;
    int map_pitch = m_qrimage->getQRImagePitch();
    int map_dim = m_qrimage->getQRImageWidth();
  
//    ret.jump_step = 0;
    
    if (ppos.x < 0 && ppos.y < 0) { // should never happen .. 
        goto done;
    }
    
    if (ppos.x < 0|| ppos.x > map_dim || ppos.y < 0 || ppos.y  > map_dim) {
        printf("Out of bound ... ");
        goto done;
    }
    
    map = m_qrimage->getMapData();
    pattern = map[ppos.x + ppos.y * map_pitch];
 
 done:       
    return pattern;
}
/*
qrutil:: checkPassthrough(pos *ppos) {


}*/

ReturnValue qrutil:: check_action(pos ppos, bool isHrzAction, bool isDiag) {
 
    int *map = NULL;
    int pattern = UNKNOWN;
    ReturnValue ret;
    int map_pitch = m_qrimage->getQRImagePitch();
    int map_dim = m_qrimage->getQRImageWidth();
  
    ret.jump_step = 0;
    
    if (ppos.x < 0 && ppos.y < 0) { // should never happen .. 
        ret.action = FINISH;
        goto done;
    }
    
    if (ppos.x < 0|| ppos.x >= map_dim || ppos.y < 0 || ppos.y  >= map_dim) {
        ret.action = (MOVE_ASIDE | CHANGE_DIR);
        goto done;
    }
    
    map = m_qrimage->getMapData();
    pattern = map[ppos.x + ppos.y * map_pitch];
        
    switch(pattern) {
        case POSITION_DETECTION:
        case FORMAT_INFO: 
            ret.action = (MOVE_ASIDE | CHANGE_DIR);
            break;
        case VERSION_INFO: 
            ret.action = (isHrzAction) ? MOVE_ASIDE: CHANGE_DIR;
 //           ret.jump_step  = (MOVE_ASIDE | );
            break;
        case ALIGNMENT:  
            ret.action = (isHrzAction) ? MOVE_ASIDE: (isDiag) ? MOVE_ASIDE: PASS_THROUGH;
            ret.jump_step = (isHrzAction) ? 0: OUTER_ALIGN_PATTERN_SIZE+1;
            break;
        case TIMING:
            ret.action = (isHrzAction) ? MOVE_ASIDE: PASS_THROUGH;
            ret.jump_step = (isHrzAction) ? 0: 2;            
            break;
        default:
            ret.action = NO_ACTION;
            break;
    }
    
done:
    return ret;
}

ReturnValue qrutil::getPathIdx(int patterns, int i, bool isHorz) {

    ReturnValue ret = {-1, 2, UNKNOWN_ACTION, NULL};
    
     printf("Before getPathIdx pattern %d i %d ret.action %d  ret.idx %d ret.jump_step %d \n", patterns, i, ret.action, ret.idx, ret.jump_step);
    switch(patterns) {
/*        case (FORMAT_INFO + UNKNOWN): 
        case (ALIGNMENT + UNKNOWN):
   //     case (TIMING + UNKNOWN):
        case (VERSION_INFO + UNKNOWN):
                ret.action = MOVE_ASIDE;
                ret.idx = i;
                break;  
   */     
        case (QUIET_ZONE + QUIET_ZONE): 
                ret.action = isHorz ? MOVE_ASIDE + CHANGE_DIR: UNKNOWN_ACTION;
                ret.idx = i;
                break; 
        case (FORMAT_INFO + FORMAT_INFO):
 //               printf("getPathIdx - MOVE_ASIDE + CHANGE_DIR %d \n", QUIET_ZONE + QUIET_ZONE);   
                ret.action = isHorz ? MOVE_ASIDE + CHANGE_DIR: UNKNOWN_ACTION;
                ret.idx = i;
                break;                       
        case (ALIGNMENT + ALIGNMENT):
                ret.action = isHorz? PASS_THROUGH: NO_ACTION;
                ret.jump_step = isHorz? OUTER_ALIGN_PATTERN_SIZE: 2;
                ret.idx = i;
                break;
        case (TIMING + TIMING):
                ret.action = isHorz? PASS_THROUGH: UNKNOWN_ACTION;
                ret.jump_step = isHorz ? 1:2;
                ret.idx = i;
                break;
//        case UNKNOWN:   
//        case (QUIET_ZONE + QUIET_ZONE):
        case (UNKNOWN + UNKNOWN):
   //             ret.action = NO_ACTION;
                     //       ret.jump_step = (isHorz(posMap[i])) ? 1: 2;
                ret.jump_step =  isHorz ? 1:2;
  //              ret.idx = i;
 //               break;
        case (UNKNOWN + UNKNOWN + UNKNOWN):
        case (UNKNOWN + UNKNOWN + UNKNOWN + UNKNOWN):
                ret.action = NO_ACTION; // 4 bits
                ret.idx = i;
                break;
        default:
                printf("getPathIdx default\n");        
                break;                
    }
    printf("getPathIdx pattern %d i %d ret.action %d ret.jump_step %d \n", patterns, i, ret.action, ret.jump_step);
    return ret;
}

bool qrutil::isHorz(std::list<pos> posMap) {
    pos tpos = posMap.front();
    bool ret = true;
 //  int tmp = firstpos.x;
    for (auto p: posMap) {
        if (tpos.y == p.y) continue;
        ret = false;
        break;
    }
    return ret;
}
void qrutil::setSymbolBit(int *map, pos cpos, std::list<pos> posMap, int dir_sign) {

   printf("setSymbolBit posMap = %d\n", posMap.size());
    for (auto p: posMap) {
        p = {cpos.x + p.x, cpos.y+dir_sign*p.y};
        printf("setSymbolBit (%d, %d) -> ", p.x, p.y);        
        int map_pitch = m_qrimage->getQRImagePitch();
        int map_dim = m_qrimage->getQRImageWidth();  
        if (p.x < QUIET_ZONE_SIZE || p.x > map_dim - QUIET_ZONE_SIZE - 1 || p.y < QUIET_ZONE_SIZE  || p.y  > map_dim - QUIET_ZONE_SIZE -1) {
            continue;
        }   
        map[p.x + p.y * map_pitch] = -5;
    }
    printf("\n");
}

ReturnValue qrutil::getNextAction(pos cpos, pos ppos[], PLACEMENT_TYPE sdir, bool isMapOdd) {
    int num_pos = sizeof(ppos)/sizeof(pos);
    pos new_pos = {};

    int *map = m_qrimage->getMapData();   
    int dir_sign = (sdir & UP) ? 1: -1;
 //   bool isHorz, isDiag;
    int i;
    ReturnValue ret =  {-1, 2, UNKNOWN_ACTION, NULL};
    int patterns = 0;
    
    std::list<pos> posMap[7] = {
   //         {{0, 0}}, 
   //         
            {{0,0}, {-1, 0}, {0, -1}, {-1, -1}},
            {{0,0}, {-1, 0}, {-1, -1}},
            {{-1,0}, {0, -1}, {-1, -1}},
            {{0,0}, {-1, 0}},
            {{0, -1}, {-1, -1}},
            {{-1, 0}, {-1, -1}},
 //           {{0,0}, {-1, 0}}
            {{0,0}, {0, -1}}

            };
/*           
    pos ppos1[] = {{0,0}, {0, -1}, {-1, -1}, {1, 0}};
    int num_pos1 = sizeof(ppos1)/sizeof(pos);
    pos *new_pos_1 = new pos [num_pos1];

    int *pattern = new int [num_pos1];
    int pathIdx=-1;
    new_pos_1[0] = {cpos.x+dir_sign*ppos1[0].x, cpos.y+dir_sign*ppos1[0].y};
    patterns = pattern[0] = getPattern(new_pos_1[0]);
    printf("pattern[0] %d \n", pattern[0]);
    for (i=1; i<num_pos1; i++) {
        new_pos_1[i] = {cpos.x+dir_sign*ppos1[i].x, cpos.y+dir_sign*ppos1[i].y};
        pattern[i] = getPattern(new_pos_1[i]);
        printf("pattern[%d] %d \n", i, pattern[i]);
        patterns += pattern[i];
        ret = getPathIdx(patterns, i);
        printf("ret.idx %d ret.action %d \n", ret.idx, ret.action);
        if (ret.idx != -1) {
            pathIdx = i;
        }        
    }    
    printf("pathIdx %d \n", pathIdx);
    if (pathIdx == -1)
        goto done;*/
    int pCounter;
    int *pattern = new int [posMap[0].size()];
    int pathIdx=-1;
//    int i;
    int listsize = sizeof(posMap)/sizeof(std::list<pos>);
    printf("listsize %d\n", listsize);
    for (i=0; i<3; i++) {
        pCounter = patterns = 0;    
        printf("Loop %d\n", i);     
        for (auto p: posMap[i]) {
            printf("(%d, %d) -> ", p.x, p.y);
            p = {cpos.x + p.x, cpos.y+dir_sign*p.y};
            
            pattern[pCounter] = getPattern(p);   
            if (pattern[pCounter] == QUIET_ZONE) {
                 printf("Hit quiet zone \n");
       //         continue;
            }
                     
            patterns+= pattern[pCounter];
            printf("(%d, %d) pattern[pCounter] %d\n", p.x, p.y, pattern[pCounter]);
            pCounter++;
           
        }
        printf("pattern %d\n", patterns);
        ret = getPathIdx(patterns, i, isHorz(posMap[i]));
        printf("ret.idx %d ret.action %d\n", ret.idx, ret.action);
        if (ret.action >= NO_ACTION) {
        pathIdx = i;
        break;
        }   
    }
    
    printf("pathIdx %d ret.jump_step %d\n", pathIdx, ret.jump_step);
    
    if (ret.action > NO_ACTION) {
        delete pattern;
        return ret;
    }
    
    if (pathIdx == -1) {
    
        for (i=3; i<7; i++) {
        pCounter = patterns = 0;    
        printf("Loop %d\n", i);     
        for (auto p: posMap[i]) {
            printf("(%d, %d) -> ", p.x, p.y);
            p = {cpos.x + p.x, cpos.y+dir_sign*p.y};
            
            pattern[pCounter] = getPattern(p); 
             
            patterns+= pattern[pCounter];
            printf("(%d, %d) pattern[pCounter] %d\n", p.x, p.y, pattern[pCounter]);
            pCounter++;
           
        }
        printf("pattern %d\n", patterns);
        ret = getPathIdx(patterns, i, isHorz(posMap[i]));
        
        if (ret.action == NO_ACTION) {
            setSymbolBit(map, cpos, posMap[i], dir_sign);
        } 
        if (ret.action & CHANGE_DIR || 
            ret.action & PASS_THROUGH ||
            ret.action == NO_ACTION ) {
            return ret;
        }

        printf("ret.idx %d ret.action %d ret.jump_step %d\n", ret.idx, ret.action, ret.jump_step);       
    }
    ret.action = NO_ACTION;
    
    return ret;
    }
    
    setSymbolBit(map, cpos, posMap[pathIdx], dir_sign);
     

    
//finish:  
 //  delete new_pos_1;
//    delete pattern;

/*
        
    for (i=0; i<num_pos; i++) {
        new_pos = {cpos.x-ppos[i].x, cpos.y+dir_sign*ppos[i].y};
        isHorz = (ppos[i].x==1 && ppos[i].y==0);
        isDiag = (ppos[i].x== -1 && dir_sign*ppos[i].y==1);
        ret = check_action(new_pos, isHorz, isDiag); 
        if (ret.action == NO_ACTION) {
            ret.cpos.x = new_pos.x; ret.cpos.y = new_pos.y;        
            break;
        } else if (ret.action & MOVE_ASIDE) {
            if (ret.action & CHANGE_DIR) {
                ret.cpos.x = cpos.x -1;
                ret.cpos.y = cpos.y;
            } else {
                ret.cpos.x = (isHorz) ? cpos.x : (isDiag) ? cpos.x: cpos.x-1;
                ret.cpos.y = (isHorz) ? cpos.y + dir_sign: : (isDiag) ?  cpos.y + dir_sign: cpos.y;                
            }
 //           ret.cpos.x = (isHorz && ((ret.action & CHANGE_DIR) == false)) ? cpos.x : cpos.x-1;
 //           ret.cpos.y = (isHorz && ((ret.action & CHANGE_DIR) == false)) ? cpos.y + dir_sign: cpos.y;
            break;
        } else if (ret.action & PASS_THROUGH) {
            // vertical only
             int  isCurOdd = (cpos.x  % 2 == 1);
            ret.cpos.x = (isMapOdd == isCurOdd) ?  cpos.x: cpos.x+1;//dir_sign*ppos[i].x;
            ret.cpos.y = cpos.y + dir_sign*ret.jump_step;
            break;
        }
    }   
    
    printf("i %d ret.cpos (%d,%d) ret.action %d cur_pos (%d, %d)  \n", i, ret.cpos.x, ret.cpos.y, ret.action, cpos.x, cpos.y);
    */
    if (ret.action & CHANGE_DIR)  {
    printf("CHANGE_DIR\n");
//    printf("ret.cpos (%d,%d) ret.action %d cur_pos (%d, %d)  \n", ret.cpos.x, ret.cpos.y, ret.action, cpos.x, cpos.y);
    }
    
 
    delete pattern;
    return ret;
    
}

 struct ReturnValue qrutil::getNextPosition(pos cpos, PLACEMENT_TYPE sdir, int map_dim, int mapPitch) { 

    int *map = m_qrimage->getMapData();
    int isMapOdd = ((map_dim -1) % 2 == 1);
    int  isCurOdd = (cpos.x  % 2 == 1);

    pos ppos[2][2] = {{{1, 0}, {0, 1}}, // same as
                             {{-1, 1}, {0, 1}} }; //odd }
    
    struct ReturnValue retValue;    
//    retValue.cdir = false;
    printf("isMapOdd %d . isCurOdd %d \n", isMapOdd, isCurOdd); 

    int sngrp = (isMapOdd != isCurOdd);   
    pos *tpos = (sdir & CHECK_ONE_MODULE_ONLY)? &ppos[sngrp][1]: ppos[sngrp];
  //  dir_sign = -1;
//    for (int i=0; i<2; i++) {
    retValue = getNextAction(cpos, tpos, sdir, isMapOdd);


/*    if (retValue.action & (NO_ACTION|CHANGE_DIR)) {
   //     retValue.cpos.x = cpos.x + dir_sign *  ppos[sngrp][i].x;
   //     retValue.cpos.y = cpos.y + dir_sign *  ppos[sngrp][i].y;
        break;
    }*/
//    }  
    //}
    
/*    if  (sdir == UP_VERTICAL) {            
    if (isMapOdd == isCurOdd) {
        if (cpos.x -1 < 0 && cpos.y -1 < 0) {
        // means finish
        printf(" Done (%d, %d) \n", cpos.x, cpos.y); 
        } else if (cpos.x -1 < 0 && cpos.y -1 > 0) {
                // move up
                retValue.cpos.x = cpos.x;
                retValue.cpos.y = cpos.y -1;
                getNextAction(retValue.cpos, false) 
        } else  {
        // last row, signal to change direction
        retValue.cpos.x = cpos.x-1;
        retValue.cpos.y = cpos.y;                        
        retValue.cdir = (cpos.y -1 < 0) ? true: retValue.cdir;
        } 
    } else {
    if (cpos.y - 1 > 0) {
    // move up
    retValue.cpos.x = cpos.x + 1;
    retValue.cpos.y = cpos.y - 1;
    } else if (cpos.x -1 < 0) {
    // last row, signal to change direction
    retValue.cpos.x = cpos.x-1;
    retValue.cpos.y = cpos.y;                        
    retValue.cdir = (cpos.y -1 < 0) ? true: retValue.cdir;
    } else {
    printf(" shouldn't come here \n"); 
    }      
    } 
*/
    /*switch(*pattern) {
        case POSITION_DETECTION :
                printf(" pattern is %d . Can write the bit here \n", pattern);  
               if  (sdir == UP_VERTICAL) {            
//                nextpos.x =(cpos.y-1 < 0) ? cpos.x-1: cpos.x ;     
//                nextpos.y = (cpos.y-1 < 0) ? cpos.y: cpos.y-1;
                if (cpos.y-1 < OUTER_POS_DETECT_PATTERN_SIZE) {
                    retValue.cpos.x = cpos.x-1;
                    retValue.cpos.y = cpos.y;
                    retValue.cdir = true;
                } else {
                    retValue.cpos.x = cpos.x ;
                    retValue.cpos.y =cpos.y-1;
                }
            } else {
                if (cpos.y+1 > map_dim - OUTER_POS_DETECT_PATTERN_SIZE) {
                    retValue.cpos.x = cpos.x-1;
                    retValue.cpos.y = cpos.y;
                    retValue.cdir = true;
                } else {
                    retValue.cpos.x = cpos.x ;
                    retValue.cpos.y =cpos.y+1;
                }  
            }       
 //           nextpos.x = (cpos.x-1 < 0) ? cpos.x: cpos.x-1; 
//            nextpos.y = (cpos.x-1 < 0) ? cpos.y-1: cpos.y;
            break;
        case ALIGNMENT:     
        case TIMING:
        case VERSION_INFO:
        case FORMAT_INFO:
        default:
            printf(" pattern is %d . Can write the bit here \n", *pattern);  
            *pattern = (*pattern == UNKNOWN) ? DATA: *pattern;
            if  (sdir == UP_VERTICAL) {            
//                nextpos.x =(cpos.y-1 < 0) ? cpos.x-1: cpos.x ;     
//                nextpos.y = (cpos.y-1 < 0) ? cpos.y: cpos.y-1;
                if (isMapOdd == isCurOdd) {
                    if (cpos.x -1 < 0 && cpos.y -1 < 0) {
                        // means finish
                        printf(" Done (%d, %d) \n", cpos.x, cpos.y); 
                    } else if (cpos.x -1 < 0 && cpos.y -1 > 0) {
                        // move up
                        retValue.cpos.x = cpos.x;
                        retValue.cpos.y = cpos.y -1;
                    } else  {
                        // last row, signal to change direction
                        retValue.cpos.x = cpos.x-1;
                        retValue.cpos.y = cpos.y;                        
                        retValue.cdir = (cpos.y -1 < 0) ? true: retValue.cdir;
                    } 
                } else {
                    if (cpos.y - 1 > 0) {
                        // move up
                        retValue.cpos.x = cpos.x + 1;
                        retValue.cpos.y = cpos.y - 1;
                    } else if (cpos.x -1 < 0) {
                        // last row, signal to change direction
                        retValue.cpos.x = cpos.x-1;
                        retValue.cpos.y = cpos.y;                        
                        retValue.cdir = (cpos.y -1 < 0) ? true: retValue.cdir;
                    } else {
                        printf(" shouldn't come here \n"); 
                    }      
                } 
                                   
           //     }
        }
            break;
    } */
    return retValue;
    
}
 int qrutil::locateCW() {
    int *map = m_qrimage->getMapData();
    int map_pitch = m_qrimage->getQRImagePitch();
    int map_dim = m_qrimage->getQRImageWidth();
    
    int i, j;    
    int isMapOdd = ((map_dim -1) % 2 == 1);
    int bitcounter, bytecounter;
    PLACEMENT_TYPE sdir = UP;
    
    pos cur_pos = {map_dim-QUIET_ZONE_SIZE-1, map_dim-QUIET_ZONE_SIZE-1};
    bitcounter = 1; bytecounter = 0;
   
   m_cword_size =  m_param->get_QRCodeWordSize(); 
   
    /*
    printf("print empty map start \n");
 //   printf("HORZ_FINFO_LOCATION %d %d m_image_dim %d\n", HORZ_FINFO_LOCATION, HORZ_FINFO_LOCATION*m_image_pitch, m_image_dim);
 //   image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location; 
    for (i=0; i<map_dim; i++) {
        for (j=0; j<map_dim; j++)
            printf("%d ",  *(map + i*map_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n");
*/
    printf("m_cword_size %d map_dim %d\n",   m_cword_size, map_dim);
    
    ReturnValue ret_value =  {-1, 2, UNKNOWN_ACTION, NULL};
  //  *(map + cur_pos.y*map_pitch + cur_pos.x) = -1;
    do {
/*        if (isMapOdd && cur_pos.x %2 == 1) {
                // check left neigbour
                if (map[cur_pos.x -1 + cur_pos.y*map_pitch] != UNKNOWN)
       } else {
                // look for upper right neigbour         
        }*/

        if (bitcounter > 0 && bitcounter % 8 == 0) {
            printf("Read one more codeword.. bytecounter %d  bitcounter %d \n", bytecounter, bitcounter);
            sdir = (sdir  & CHECK_ONE_MODULE_ONLY) ? (PLACEMENT_TYPE)(sdir | ~CHECK_ONE_MODULE_ONLY): sdir;
            bytecounter++;
        }       

        printf("cur_pos (%d,%d) writable %d sdir %d \n", cur_pos.x, cur_pos.y, map[cur_pos.x+cur_pos.y*map_pitch] == UNKNOWN, sdir);
                
        if (bytecounter > 32)  {
            printf("Investigate here\n");
        }
        ret_value = getNextPosition(cur_pos, sdir, map_dim, map_pitch);    
        
//        if (cur_pos.y < map_dim - QUIET_ZONE_SIZE || cur_pos.y > QUIET_ZONE_SIZE)
//            cur_pos.y= (sdir & UP) ? cur_pos.y - 2: cur_pos.y + 2;
            
    //  if (ret_value.action & NO_ACTION)
  /*     {
            map[ret_value.cpos.x+ret_value.cpos.y*map_pitch] = DATA +bytecounter;
       }     */
 //       memcpy(&cur_pos, &ret_value.cpos, sizeof(pos));
 
         printf("before ret_value.action %d\n",ret_value.action);
         if ((ret_value.action & CHANGE_DIR) == 0) {

           printf("after ret_value.action %d %d %d \n",ret_value.action, CHANGE_DIR + MOVE_ASIDE, CHANGE_DIR & MOVE_ASIDE);
        }


        if (ret_value.action & CHANGE_DIR) {
            bool onemodule = (sdir & CHECK_ONE_MODULE_ONLY);
            sdir = (sdir & UP) ? DOWN: UP;
            sdir = (onemodule) ? (PLACEMENT_TYPE) (sdir | CHECK_ONE_MODULE_ONLY): sdir;
            cur_pos.y =  (sdir & UP) ? cur_pos.y - 1:  cur_pos.y + 1;
        } else if (ret_value.action & PASS_THROUGH) {
            cur_pos.y =  (sdir & UP) ? cur_pos.y - ret_value.jump_step:  cur_pos.y  + ret_value.jump_step;
        }
        else {
             if (sdir & UP) {
                if ((cur_pos.y < (map_dim - QUIET_ZONE_SIZE))  && (cur_pos.y >= QUIET_ZONE_SIZE + ret_value.jump_step)) {
                 cur_pos.y -=  ret_value.jump_step;
//                } else if ((cur_pos.y < (map_dim - QUIET_ZONE_SIZE))  && cur_pos.y > QUIET_ZONE_SIZE  && cur_pos.y < QUIET_ZONE_SIZE + ret_value.jump_step) {
//                 cur_pos.y  = QUIET_ZONE_SIZE;
                 } else {
                    ret_value.action = MOVE_ASIDE;
                    sdir = DOWN;
                    cur_pos.y -=1;
                } 
            } else {   
                if ((cur_pos.y < (map_dim - QUIET_ZONE_SIZE - ret_value.jump_step))  && cur_pos.y >= QUIET_ZONE_SIZE) {
                    cur_pos.y += ret_value.jump_step;// 2;
                } else {    
                     ret_value.action = MOVE_ASIDE;
                     sdir = UP;
                     cur_pos.y += 1;
               }
           }
        
        }
          /* else {
            printf("ret_value.cpos (%d,%d) ret_value action %d cur_pos (%d, %d)  \n", ret_value.cpos.x, ret_value.cpos.y, ret_value.action, cur_pos.x, cur_pos.y);
            if (cur_pos.y < map_dim - QUIET_ZONE_SIZE  && cur_pos.y >= QUIET_ZONE_SIZE)
                 cur_pos.y =  (sdir & UP) ? cur_pos.y - 2: cur_pos.y + 2;
             else {
                 sdir = (sdir & UP) ? DOWN: UP;
                 ret_value.action = MOVE_ASIDE;
//                 if (cur_pos.x > QUIET_ZONE_SIZE) cur_pos.x -= 2;
             }*/
        //    if (sdir & DOWN && cur_pos.y < map_dim - QUIET_ZONE_SIZE - 2 &&  cur_pos.y > QUIET_ZONE_SIZE)
        //        cur_pos.y = cur_pos.y + 2;
       // }
  
 //       if (sdir & UP && cur_pos.y < map_dim - QUIET_ZONE_SIZE - 2  && cur_pos.y > QUIET_ZONE_SIZE + 2)
 //           cur_pos.y -= 2;
            
        if (ret_value.action & MOVE_ASIDE && cur_pos.x >= QUIET_ZONE_SIZE)// ||
//            cur_pos.x > QUIET_ZONE_SIZE && cur_pos.y >= map_dim - QUIET_ZONE_SIZE  || 
//            cur_pos.x > QUIET_ZONE_SIZE && cur_pos.y <= QUIET_ZONE_SIZE)
            cur_pos.x -= 2;
        printf("next cur_pos (%d,%d) writable %d  bitcounter %d \n", cur_pos.x, cur_pos.y, map[cur_pos.x+cur_pos.y*map_pitch] == UNKNOWN,  bitcounter);
        bitcounter++;
    } while (bytecounter < m_cword_size);
    
    

     printf("===print empty image start \n");
 //   image_ptr =  (unsigned char *) m_qrBaseMemory +m_item_pointer.image_location;
    for (i=0; i<map_dim; i++) {
        for (j=0; j<map_dim; j++)
            printf("%3d ", *(map + i*map_pitch + j));
        printf("\n");
    } 
    printf("print empty image ends \n"); 
 }
  /*
  bool detectUsedBits(pos startpos) {
        int i, j;
        pos endpos; // top left corner of the search window
        int actual_window_x, actual_window_y;
        int *sub_map_ptr = NULL;// = map + startpos.y * map_pitch + startpos.x;
        
        endpos.x = (startpos.x-m_swin_size > 0) ? startpos.x-m_swin_size: 0; 
        endpos.y = (startpos.y-m_swin_size > 0) ? startpos.y-m_swin_size: 0; 
        actual_window_x = startpos.x-endpos.x; actual_window_y = startpos.y-endpos.x; 
        
        for (i=0 i>actual_window_y; i++) {
            sub_map_ptr = map + (startpos.y - i) * map_pitch + startpos.x;
            for (j=0; j<actual_window_x; j++) {
                if (sub_map_ptr[j] != UNINITIALIZED) break;
            }
        }
  }
  
int locateOneCW() {
    return 0;
}*/
 
 

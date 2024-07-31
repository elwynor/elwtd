/*****************************************************************************
 *   ELWTD.C      V2.0                                 T H I N K D I C E !   *
 *                                                                           *
 *   Copyright (C) 1989,2004,2024 Rick Hadsall.  All Rights Reserved.        *
 *                                                                           *
 *   Description:       ThinkDice! - BBS Random Dice Game                    *
 *                 Asks user to guess the outcome of the dice roll           *
 *                   *** A fun, thoughtful ESP-like game! ***                *
 *                                                                           *
 *                                         - v5.x code R. Hadsall 01/03/90   *
 *                                         - v6.x code R. Hadsall 05/18/2004 *
 *                                         - WG32 code R. Hadsall 02/20/2006 *
 *                                         - v10  code R. Hadsall 04/17/2023 *
 *                   - text file to btrieve conversion R. Hadsall 07/28/2024 *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      * 
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/
#include "gcomm.h"
#include "majorbbs.h"
#include "elwtd.h"

/* function declarations */
VOID EXPORT init__elwtd(VOID);
GBOOL londic(VOID);
GBOOL tdicerou(VOID);
VOID initrnd(VOID);
INT getrnd(INT low,INT high);
VOID deldice(CHAR *whotodelete);
VOID clsdice(VOID);
VOID showtopten(VOID);
VOID storeGameRecords(CHAR *who,SHORT gamescore,SHORT numcorrect);
VOID storeScoreRecord(CHAR *who,SHORT gamescore,SHORT numcorrect);
VOID storeUserRecord(CHAR *who,SHORT gamescore);
INT getRank(SHORT newscore);

INT tdstt;					      /* User state when in ThinkDice!        */
struct module elwtd={         /* module interface block               */
     "",                      /*    name used to refer to this module */
     londic,                  /*    user logon supplemental routine   */
     tdicerou,                /*    input routine if selected         */
     NULL,                    /*    status-input routine if selected  */
     NULL,                    /*    "injoth" routine for this module  */
     NULL,                    /*    user logoff supplemental routine  */
     NULL,                    /*    hangup (lost carrier) routine     */
     NULL,                    /*    midnight cleanup routine          */
     deldice,                 /*    delete-account routine            */
     clsdice                  /*    finish-up (sys shutdown) routine  */
};

INT tdicemsv;                 /* Display Logon Message ?              */

HMCVFILE tdmb;                /* Message file                         */
DFAFILE *tdsdat,              /* Game scores data file                */
        *tdudat;              /* User record data file                */

// elwtdusr.dat
struct tduser {
   CHAR name[UIDSIZ];         /* userid                            K0 */
   LONG score;                /* aggregate score from all games    K1 */
#if defined(IS30) || defined(__BUILDV10MODULE)
   INT  gamesplayed;          /* number of games played            K2 */
#else
   LONG gamesplayed;          /* number of games played            K2 */
#endif
}; // size 38

// elwtdsco.dat
struct tdscore {
   CHAR name[UIDSIZ];         /* userid                            K0 */
   SHORT score;               /* total score - max 16x5=80         K1 */
   SHORT cgs;                 /* total correct guesses - max 5     K2 */
   time_t gamedate;           /* date/time for the game score      K3 */
}; // size 38

// Game variables
SHORT right,               
      wrong,
      score,
      guess,
      diceroll,
      rollnum,
      plspts,
      minpts;

#define VERSION "2.00"

void EXPORT
init__elwtd()                 /* initialize ThinkDice!                */
{
     // register the module
     stzcpy(elwtd.descrp,gmdnam("ELWTD.MDF"),MNMSIZ);
     tdstt=register_module(&elwtd);

     // open the message file
     tdmb=opnmsg("elwtd.mcv");
     tdicemsv=ynopt(TDICEMSV);

     // open the data files
     tdsdat=dfaOpen("elwtdsco.dat",sizeof(struct tdscore),NULL);
     tdudat=dfaOpen("elwtdusr.dat",sizeof(struct tduser), NULL);

     initrnd();

     shocst(spr("ELW ThinkDice v%s",VERSION),"(C) Copyright 2004-24 Elwynor Technologies - www.elwynor.com");
}

GBOOL
londic(VOID)                  /* User Logon Message                   */
{
   if (tdicemsv) { 
       setmbk(tdmb);
       prfmsg(LOGIN);
       outprf(usrnum);
       rstmbk();
   } 

   return(0);
}

GBOOL
tdicerou(VOID)                /* ThinkDice main input function        */
{
    INT win;          

    setmbk(tdmb);
    switch (usrptr->substt) {
    case 0:
         prfmsg(usrptr->substt=MMENU);
         break;
    case MMENU:
         if (sameas(margv[0],"x")) {
           prfmsg(LEAVE,usaptr->userid);
           return(0);
         }
         else if (sameas(margv[0],"?")) {
           prfmsg(MMENU);
           break;
         }
         else if (sameas(margv[0],"\0") || sameas(margv[0],"")) {                 
         }
         else if (sameas(margv[0],"i")) {
           prfmsg(DICEINS);
         }
         else if (sameas(margv[0],"t")) {
           showtopten();
         }
         else if (sameas(margv[0],"p")) {
              score = right = wrong = 0;
              rollnum = 1;
              if (!hasmkey(TDICEKEY)) {
                prfmsg(NOTLIV,usaptr->userid);
              }
              else {
                prfmsg(usrptr->substt=ROLL,rollnum);
                break;
              }
         }
         else {                          /* Selection Invalid */
              prfmsg(NOGOOD);
         }
         prfmsg(SMENU);
         break;
    case ROLL: /* Playing game state */
         if (atoi(margv[0]) > 0 && atoi(margv[0]) < 7) { // || sameas(margv[0],"0")) {
           guess=(SHORT)atoi(margv[0]);                
           diceroll = (SHORT)getrnd(1,6);
           prfmsg(DIE1+(diceroll-1));

           if (diceroll == guess) {
             right += 1;
             plspts = diceroll + 10;
             score += plspts;
             prfmsg(CORRECT,usaptr->userid,diceroll,plspts,score);
           }
           else {
             wrong += 1;
             minpts = diceroll + 2;
             score -= minpts;
             prfmsg(WRONG,usaptr->userid,diceroll,guess,minpts,score);
           }
           rollnum++;
           if (rollnum == 6) {
             prfmsg(ENDGAME,usaptr->userid,score,right,wrong);
             if (score >= 0) win=getRank(score);
             else win=11;
             if (win < 11) {
               prfmsg(WINNING);
             }
             storeGameRecords(usaptr->userid,score,right);
             prfmsg(SMENU);
             usrptr->substt=MMENU;
             break;       
           } 
         }
         else if (sameas(margv[0],"x")) {
           prfmsg(ABORT);
           prfmsg(SMENU);
           usrptr->substt=MMENU;
           break;
         }
         else if (margc==0) {
         }
         else {
           prfmsg(BADGUESS);
         }
         prfmsg(ROLL,rollnum);
         break;
    }
    outprf(usrnum);
    return(1);
}

INT
getRank(SHORT newscore)       /* see if the score is a top ten score  */
{
   INT rank=1;
   struct tdscore tds;
   
   dfaSetBlk(tdsdat);
   if ((dfaAcqHI(&tds,1)) == 1) {
     if (newscore < tds.score) {
        rank=1;
        while (dfaQueryPR()) {
           dfaAbsRec(&tds,1);
           rank++;
           if (newscore >= tds.score) break;
        }
     }
   }
   
   return(rank);
}

VOID
storeUserRecord(CHAR *who,SHORT gamescore)  /* create/update the user's cumulative record */
{
   struct tduser tdu;
   
   dfaSetBlk(tdudat);
   if (dfaAcqEQ(&tdu,who,0)) {
      tdu.score += gamescore;
      tdu.gamesplayed += 1;
      dfaUpdate(&tdu);
   } else {
      setmem(&tdu,sizeof(struct tduser),0);
      strcpy(tdu.name,who);
      tdu.score=gamescore;
      tdu.gamesplayed=1;
      dfaInsert(&tdu);
   }
   
}

VOID
storeScoreRecord(CHAR *who,SHORT gamescore,SHORT numcorrect) /* insert the game score record */
{
   struct tdscore tds;
   
   dfaSetBlk(tdsdat);
   setmem(&tds,sizeof(struct tdscore),0);
   strcpy(tds.name,who);
   tds.score=gamescore;
   tds.cgs=numcorrect;
   tds.gamedate=now();
   dfaInsertDup(&tds);
}

VOID
storeGameRecords(CHAR *who,SHORT gamescore,SHORT numcorrect)  /* store/update records from the game session */
{
   storeUserRecord(who,gamescore);
   storeScoreRecord(who,gamescore,numcorrect);
}

VOID showtopten(VOID) {       /* display the top 10 valid scores      */
   
   struct tdscore tds; 
   SHORT x;
   
   setmbk(tdmb);
   prfmsg(TOPTEN);
   dfaSetBlk(tdsdat);
   if (dfaAcqHI(&tds,1)) {
     x=1;
     if (tds.score >= 0) {
       prfmsg(TOPS,tds.name,spr("%ld",tds.score),spr("%ld",tds.cgs));
       while (dfaQueryPR()) {
         dfaAbsRec(&tds,1);
         x++;
         if (tds.score >= 0) {
            prfmsg(TOPS,tds.name,spr("%ld",tds.score),spr("%ld",tds.cgs));
         } else {
            prfmsg(TOPS,"---","0","0");
         }
         if (x == 11) break;        
       }
       if (x < 11) {
         while (x < 11) {
            prfmsg(TOPS,"---","0","0");
            x++;            
         }
       }
     } else {
       prfmsg(NOHIGHS);
     }
      
   } else {
      prfmsg(NOSCORES);
   }
   outprf(usrnum);
}

VOID
initrnd(VOID)                 /* initialize random number seed        */
{
#ifdef __BUILDV10MODULE  
  srand((unsigned)time(NULL)); // BBSV10
#else
  randomize(); // WG32
#endif
}

INT
getrnd(INT lo,INT hi)         /* Returns random # between lo and hi   */
{
#ifdef __BUILDV10MODULE
    return(lo + rand() % (hi - lo + 1)); 
#else
    return (random(hi - lo + 1) + lo);   
#endif
}

VOID
clsdice(VOID)                 /* close MSG and DAT files for shutdown */
{
  clsmsg(tdmb);
  dfaClose(tdsdat);
  dfaClose(tdudat);
}

VOID
deldice(CHAR *whotodelete)    /* delete user from DAT files on delete */
{
  dfaSetBlk(tdsdat);
  while (dfaAcqEQ(NULL,whotodelete,0)) dfaDelete();
  dfaRstBlk();
  
  dfaSetBlk(tdudat);
  if (dfaAcqEQ(NULL,whotodelete,0)) dfaDelete();
  dfaRstBlk();
}

#include <windows.h>
#include <math.h>
#pragma hdrstop
#include "common.h"
#include "inter.h"

// Zarro 2020 - Define gui rectangles as arrays - easier to find and change them here (sort of)
int gui_inv_up[] 	= { 600,   5, 612,  35 };
int gui_inv_down[]	= { 600,  76, 612, 106 };

int gui_skl_up[]	= { 233, 117, 245, 152 };
int gui_skl_down[]	= { 233, 222, 245, 256 };
int gui_update[]	= { 136, 257, 185, 270 };
int gui_skl_pm[]	= { 160,   6, 184, 255 };
int gui_skl_names[]	= {   6, 118, 135, 255 };

int gui_f_col[]		= {1080 };
int gui_f_row[]		= { 662, 678, 694, 587, 602, 617, 632 };

int gui_trash[]		= { 535, 111 };
int gui_coin[]		= { 437, 143 };

//int gui_coin_x[]	= { 393, 425, 457, 489, 521, 553, 585 };
//int gui_coin_y[]	= {  77 };

int gui_inv_x[]		= { 260, 599 };
int gui_inv_y[]		= {   5, 106 };

//					   HEAD,NECK,BODY,ARMS,BELT,CHRM,FEET,LHND,RHND,CLOK,LRNG,RRNG,CHRM2
int gui_equ_x[]		= { 738, 700, 738, 704, 738, 777, 738, 806, 670, 772, 776, 700, 801 };
int gui_equ_y[]		= {   5,  18,  39,  56,  73,  17, 107,  56,  56,  56,  94,  94,  17 };

// Back to the regular Borland defines
extern int init_done;
extern unsigned int inv_pos,skill_pos;
extern unsigned int look_nr,look_type;
extern unsigned char inv_block[];
extern int tile_x,tile_y,tile_type;

extern int screen_width, screen_height, screen_tilexoff, screen_tileyoff, screen_viewsize, view_subedges;
extern int xwalk_nx, xwalk_ny, xwalk_ex, xwalk_ey, xwalk_sx, xwalk_sy, xwalk_wx, xwalk_wy;
extern short screen_windowed;
extern short screen_renderdist;

extern int xoff,yoff;
extern int stat_raised[];
extern int stat_points_used;
extern int noshop;
extern int do_alpha;

void dd_invalidate_alpha(void);

int hightlight=0;
int hightlight_sub=0;
int cursor_type=CT_NONE;
int selected_char=0;
int last_skill=-1;

int xmove=0;

void cmd(int cmd,int x,int y);
void cmd3(int cmd,int x,int y,int z);

int mouse_x=0,mouse_y=0;

int trans_button(int x,int y)
{
	int n;
	int tx,ty;

	// Scroll for Inventory
	if (	x>gui_inv_up[RECT_X1] 	&& y>gui_inv_up[RECT_Y1] 
		&&  x<gui_inv_up[RECT_X2] 	&& y<gui_inv_up[RECT_Y2]) return 12;
	if (	x>gui_inv_down[RECT_X1] && y>gui_inv_down[RECT_Y1] 
		&&  x<gui_inv_down[RECT_X2] && y<gui_inv_down[RECT_Y2]) return 13;
	
	// Scroll for Skill List
	if (	x>gui_skl_up[RECT_X1] 	&& y>gui_skl_up[RECT_Y1] 
		&&  x<gui_skl_up[RECT_X2] 	&& y<gui_skl_up[RECT_Y2]) return 14;
	if (	x>gui_skl_down[RECT_X1] && y>gui_skl_down[RECT_Y1] 
		&&  x<gui_skl_down[RECT_X2] && y<gui_skl_down[RECT_Y2]) return 15;
	
	// Fast, Normal, Slow, Health
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[0];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n;
			cursor_type=CT_NONE;
			return n;
		}
		tx-=49;
	}

	// _, Hide, Names, _
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[1];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n+4;
			cursor_type=CT_NONE;
			return n+4;
		}
		tx-=49;
	}

	// _, _, _, Exit
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[2];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n+8;
			cursor_type=CT_NONE;
			return n+8;
		}
		tx-=49;
	}
	
	// First skill shortcut bar row
  	tx=x-gui_f_col[0];
	ty=y-gui_f_row[3];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n;
			cursor_type=CT_NONE;
			return n+16;
		}
		tx-=49;
	}
	
	// Second skill shortcut bar row
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[4];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n+4;
			cursor_type=CT_NONE;
			return n+20;
		}
		tx-=49;
	}
	
	// Third skill shortcut bar row
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[5];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n+8;
			cursor_type=CT_NONE;
			return n+24;
		}
		tx-=49;
	}
	
	// Fourth skill shortcut bar row
	tx=x-gui_f_col[0];
	ty=y-gui_f_row[6];
	for (n=0; n<4; n++) 
	{
		if (tx>=0 && tx<=41 && ty>=0 && ty<=14) 
		{
			hightlight=HL_BUTTONBOX;
			hightlight_sub=n+8;
			cursor_type=CT_NONE;
			return n+28;
		}
		tx-=49;
	}
	
	// New stat boxes
	if ( x>260 && y>117+14*0 && x<426 && y<130+14*0 ) return 32;
	if ( x>260 && y>117+14*1 && x<426 && y<130+14*1 ) return 33;
	if ( x>260 && y>117+14*2 && x<426 && y<130+14*2 ) return 34;
	if ( x>260 && y>117+14*3 && x<426 && y<130+14*3 ) return 35;
	if ( x>260 && y>117+14*4 && x<426 && y<130+14*4 ) return 36;
	if ( x>260 && y>117+14*5 && x<426 && y<130+14*5 ) return 37;

	return -1;
}

void button_command(int nr)
{
	int sk, keys;
	
	keys=0;
	if (GetAsyncKeyState(VK_SHIFT)&0x8000) keys|=1;
	if (GetAsyncKeyState(VK_CONTROL)&0x8000) keys|=2;

	switch (nr) 
	{
		// Row 1 of F buttons
		case  0: cmd(CL_CMD_MODE,2,0); break;
		case  1: cmd(CL_CMD_MODE,1,0); break;
		case  2: cmd(CL_CMD_MODE,0,0); break;
		case  3: pdata.show_proz=1-pdata.show_proz; break;
		// Row 2 of F buttons
		case  4: pdata.show_stats=1-pdata.show_stats; break; // do_alpha=1-do_alpha; dd_invalidate_alpha(); break;
		case  5: pdata.hide=1-pdata.hide; break;
		case  6: pdata.show_names=1-pdata.show_names; break;
		case  7: pdata.show_bars=1-pdata.show_bars; break;
		// Row 3 of F buttons
		case  8: break;
		case  9: break;
		case 10: break;
		case 11: xmove=0; cmd_exit(); break; // exit

		// Scroll bar for inventory
		case 12: 
			if (keys)
			{
				inv_pos = 0; 
			}
			else
			{
				if (inv_pos> 1)	inv_pos -= 10; 
			}
			break;
		case 13: 
			if (keys)
			{
				inv_pos = 10; 
			}
			else
			{
				if (inv_pos<10)	inv_pos += 10; 
			}
			break;

		// Scroll bar for skill list
		case 14: 
			if (keys)
			{
				if (skill_pos>11)	skill_pos -= 10; 
				else 				skill_pos = 0;
			}
			else
			{
				if (skill_pos> 1)	skill_pos -= 2; 
			}
			break;
		case 15: 
			if (keys)
			{
				if (skill_pos<30)	skill_pos += 10; 
				else				skill_pos = 40;
			}
			else
			{
				if (skill_pos<40)	skill_pos += 2; 
			}
			break;
		
		// Spell hotkeys
		case 16: case 17: case 18: case 19: 
		case 20: case 21: case 22: case 23: 
		case 24: case 25: case 26: case 27: 
		case 28: case 29: case 30: case 31: 
			if ((sk=pdata.xbutton[nr-16].skill_nr)!=-1) 
			{
				cmd3(CL_CMD_SKILL,sk,selected_char,1);
            } 
			else 
				xlog(1,"This button is unassigned. Right click on a skill/spell, then right click on the button to assign it.");
			break;

		default: break;
	}
}

void button_help(int nr)
{
	char tmp;
	
	switch (nr)
	{
		// Row 1 of F button skills
		case  0: 
			xlog(1,"F1/FAST: Makes you move faster, but uses more endurance. You will also be seen more easily."); 
			break;
		case  1: 
			xlog(1,"F2/NORMAL: Move at normal speed. Takes no endurance when not fighting.");
			break;
		case  2: 
			xlog(1,"F3/SLOW: Makes you move slower. You regain endurance while not fighting. Decreases chances of being seen.");
			break;
		case  3: 
			xlog(1,"F4/HEALTH: Toggle whether the current health of all characters is displayed as a percent.");
			break;
		
		// Row 2 of F buttons
		case  4: 
			xlog(1,"F5/STATS: Toggle seeing stat bases on the skill list, displayed as blue numbers.");
			break;
		case  5: 
			xlog(1,"F6/HIDE: Toggle whether walls and other high objects are displayed.");
			break;
		case  6: 
			xlog(1,"F7/NAMES: Toggle whether the name of all characters is displayed.");
			break;
		case  7: 
			xlog(1,"F8/BARS: Toggle whether the current health of all characters is displayed as bars.");
			break;
		
		// Row 3 of F buttons
		case  8: break;
		case  9: break;
		case 10: break;
		case 11: 
			xlog(1,"F12/EXIT: Leave the game immediately. Will cost you 50%% health and can result in death."); 
			break;
		
		// Scroll bar for inventory
		case 12: xlog(1,"Scroll inventory contents up."); break;
		case 13: xlog(1,"Scroll inventory contents down"); break;
		
		// Scroll bar for skill list
		case 14: xlog(1,"Scroll skill list up"); break;
		case 15: xlog(1,"Scroll skill list down"); break;
		
		// Spell hotkeys
		case 16: case 17: case 18: case 19:
		case 20: case 21: case 22: case 23:
		case 24: case 25: case 26: case 27: 
		case 28: case 29: case 30: case 31: 
			if (last_skill > -1 && last_skill < 50) 
			{
				switch (nr)
				{
					case 16: tmp = '1'; break; case 17: tmp = '2'; break; case 18: tmp = '3'; break; case 19: tmp = '4'; break;
					case 20: tmp = 'Q'; break; case 21: tmp = 'W'; break; case 22: tmp = 'E'; break; case 23: tmp = 'R'; break;
					case 24: tmp = 'A'; break; case 25: tmp = 'S'; break; case 26: tmp = 'D'; break; case 27: tmp = 'F'; break;
					case 28: tmp = 'Z'; break; case 29: tmp = 'X'; break; case 30: tmp = 'C'; break; case 31: tmp = 'V'; break;
					default: tmp = '?'; break;
				}
				
				if (skilltab[last_skill].nr==pdata.xbutton[nr-16].skill_nr) 
				{
					pdata.xbutton[nr-16].skill_nr=-1;
					strcpy(pdata.xbutton[nr-16].name,"-");
					xlog(1,"CTRL-%c, now unassigned.", tmp);
				} 
				else 
				{
					pdata.xbutton[nr-16].skill_nr=skilltab[last_skill].nr;
					xlog(1,"CTRL-%c, now %s.", tmp, skilltab[last_skill].name);
					strncpy(pdata.xbutton[nr-16].name,skilltab[last_skill].name,7);
					pdata.xbutton[nr-16].name[7]=0;
				}
			}
			else
				xlog(1,"Right click on a skill/spell first to assign it to a button.");
			break;

		case 32: // Need to adjust these to reflect last_skill value from 50 to 54...
			xlog(1,"DPS is the average of your damage per hit, times your attack speed. Does not account for bonus damage from weapon skill."); 
			break;
		case 33: 
			xlog(1,"DPH ranges from 1/4 of (your Weapon Value), to 1/4 of (your Weapon Value, plus half strength, plus 14, times your average crit chance)."); 
			break;
		case 34: 
			xlog(1,"Attacks per second. This is determined by your Agility score and any increases to action speed."); 
			break;
		case 35: 
			xlog(1,"Cooldown Rate is the rate at which Skill Exhaustion is removed. Determined by your Intuition score."); 
			break;
		case 36: 
			xlog(1,"Spell Modifier is determined by your character class and affects all spells."); 
			break;
		case 37: 
			xlog(1,"Spell Aptitude is how powerful a spell you can receive from any source. Determined by Willpower, Intuition, and Spell Modifier."); 
			break;

		default: break;
	}
}

void reset_block(void)
{
	int n;

	if (pl.citem) {
		if (pl.citem_p&PL_HEAD) inv_block[WN_HEAD]=0;
		else inv_block[WN_HEAD]=1;
		if (pl.citem_p&PL_NECK) inv_block[WN_NECK]=0;
		else inv_block[WN_NECK]=1;
		if (pl.citem_p&PL_BODY) inv_block[WN_BODY]=0;
		else inv_block[WN_BODY]=1;
		if (pl.citem_p&PL_ARMS) inv_block[WN_ARMS]=0;
		else inv_block[WN_ARMS]=1;
		if (pl.citem_p&PL_BELT) inv_block[WN_BELT]=0;
		else inv_block[WN_BELT]=1;
		if (pl.citem_p&PL_CHARM) inv_block[WN_CHARM]=inv_block[WN_CHARM2]=0;
		else inv_block[WN_CHARM]=inv_block[WN_CHARM2]=1;
		if (pl.citem_p&PL_FEET) inv_block[WN_FEET]=0;
		else inv_block[WN_FEET]=1;
		if (pl.citem_p&PL_WEAPON) inv_block[WN_RHAND]=0;
		else inv_block[WN_RHAND]=1;
		if (pl.citem_p&PL_SHIELD) inv_block[WN_LHAND]=0;
		else inv_block[WN_LHAND]=1;
		if (pl.citem_p&PL_CLOAK) inv_block[WN_CLOAK]=0;
		else inv_block[WN_CLOAK]=1;
		if (pl.citem_p&PL_RING) inv_block[WN_LRING]=inv_block[WN_RRING]=0;
		else inv_block[WN_LRING]=inv_block[WN_RRING]=1;
	} 
	else 
	{
		for (n=0; n<20; n++) inv_block[n]=0;
	}
	if (pl.worn_p[WN_RHAND]&PL_TWOHAND) inv_block[WN_LHAND]=1;
}

int mouse_inventory(int x,int y,int mode)
{
	int nr,keys;
	int tx,ty;
	int n;

	keys=0;
	if (GetAsyncKeyState(VK_SHIFT)&0x8000) keys|=1;
	if (GetAsyncKeyState(VK_CONTROL)&0x8000) keys|=2;
	
	// money
	if (x>gui_coin[0] && x<gui_coin[0]+34 && y>gui_coin[1] && y<gui_coin[1]+34) 
	{
		if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,100000,selected_char);
		if (mode==MS_RB_UP) xlog(1,"1,000 gold coins.");
		return 1;
	}
	
	/*
	if (y>gui_coin_y[0] && y<gui_coin_y[0]+28)
	{
		if (x>gui_coin_x[0] && x<gui_coin_x[0]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,1,selected_char);
			if (mode==MS_RB_UP) xlog(1,"1 silver coin.");
			hightlight=HL_MONEY;
			hightlight_sub=1;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[1] && x<gui_coin_x[1]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,10,selected_char);
			if (mode==MS_RB_UP) xlog(1,"10 silver coins.");
			hightlight=HL_MONEY;
			hightlight_sub=2;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[2] && x<gui_coin_x[2]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,100,selected_char);
			if (mode==MS_RB_UP) xlog(1,"1 gold coin.");
			hightlight=HL_MONEY;
			hightlight_sub=3;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[3] && x<gui_coin_x[3]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,1000,selected_char);
			if (mode==MS_RB_UP) xlog(1,"10 gold coins.");
			hightlight=HL_MONEY;
			hightlight_sub=4;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[4] && x<gui_coin_x[4]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,10000,selected_char);
			if (mode==MS_RB_UP) xlog(1,"100 gold coins.");
			hightlight=HL_MONEY;
			hightlight_sub=5;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[5] && x<gui_coin_x[5]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,100000,selected_char);
			if (mode==MS_RB_UP) xlog(1,"1,000 gold coins.");
			hightlight=HL_MONEY;
			hightlight_sub=1;
			cursor_type=CT_TAKE;
			return 1;
		}
		if (x>gui_coin_x[6] && x<gui_coin_x[6]+28) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,2,1000000,selected_char);
			if (mode==MS_RB_UP) xlog(1,"10,000 gold coins.");
			hightlight=HL_MONEY;
			hightlight_sub=2;
			cursor_type=CT_TAKE;
			return 1;
		}
	}
	*/
 	
	// trashbin
	if (x>gui_trash[0] && x<gui_trash[0]+34 && y>gui_trash[1] && y<gui_trash[1]+34) 
	{
		if (mode==MS_LB_UP) cmd3(CL_CMD_INV,9,0,selected_char);
		if (mode==MS_RB_UP) xlog(1,"Dispose of items under your cursor here.");
		return 1;
	}
	
	// backpack
	if (x>gui_inv_x[0] && x<gui_inv_x[1] && y>gui_inv_y[0] && y<gui_inv_y[1]) 
	{
		tx=(x-gui_inv_x[0])/34;
		ty=(y-gui_inv_y[0])/34;

		nr=tx+ty*10;
		if (keys==1) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,0,nr+inv_pos,selected_char);
			else if (mode==MS_RB_UP) cmd3(CL_CMD_INV_LOOK,nr+inv_pos,0,selected_char);
			if (pl.item[nr+inv_pos]) 
			{
				if (pl.citem) cursor_type=CT_SWAP;
				else cursor_type=CT_TAKE;
			} else 
			{
				if (pl.citem) cursor_type=CT_DROP;
				else cursor_type=CT_NONE;
			}
		}
		else if (keys==0) 
		{
			if (mode==MS_LB_UP) cmd3(CL_CMD_INV,6,nr+inv_pos,selected_char);
			else if (mode==MS_RB_UP) cmd3(CL_CMD_INV_LOOK,nr+inv_pos,0,selected_char);
			if (pl.item[nr+inv_pos]) cursor_type=CT_USE;
			else cursor_type=CT_NONE;
		} 
		else 
			cursor_type=CT_NONE;
		hightlight=HL_BACKPACK;
		hightlight_sub=nr+inv_pos;
		return 1;
	}

	// worn
	for (n = 0; n < 13; n++)
	{
		if (x>gui_equ_x[ n ] && x<gui_equ_x[ n ]+33 && y>gui_equ_y[ n ] && y<gui_equ_y[ n ]+33) 
		{
			if (keys==1) 
			{
				if 			(mode==MS_LB_UP) cmd3(CL_CMD_INV,1, n ,selected_char);
				else if 	(mode==MS_RB_UP) cmd3(CL_CMD_INV,7, n ,selected_char);
				if (pl.worn[ n ]) {	if (pl.citem) cursor_type=CT_SWAP; else cursor_type=CT_TAKE; } 
				else { 					if (pl.citem) cursor_type=CT_DROP; else cursor_type=CT_NONE; }
			} 
			else if (keys==0) 
			{
				if 			(mode==MS_LB_UP) cmd3(CL_CMD_INV,5, n ,selected_char);
				else if 	(mode==MS_RB_UP) cmd3(CL_CMD_INV,7, n ,selected_char);
				if (pl.worn[ n ]) cursor_type=CT_USE; else cursor_type=CT_NONE;
			} 
			else { cursor_type=CT_NONE; }
			hightlight=HL_EQUIPMENT;
			hightlight_sub= n ;
			return 1;
		}
	}
	
	return 0;
}

int mouse_buttonbox(int x,int y,int state)
{
	int nr;

	nr=trans_button(x,y);
	if (nr==-1) return 0;

	if (state==MS_LB_UP) button_command(nr);
	if (state==MS_RB_UP) button_help(nr);

	return 1;
}

int _mouse_statbox(int x,int y,int state)
{
	int n,m;

	if (screen_windowed == 1) {
        y-=3;
    }
	else
	{
		y-=1;
	}

	// Update Button
	if (	x>gui_update[RECT_X1] 	&& y>gui_update[RECT_Y1] 
		&&  x<gui_update[RECT_X2] 	&& y<gui_update[RECT_Y2])
	{
		hightlight=HL_STATBOX;
		hightlight_sub=0;
		if (state==MS_RB_UP) xlog(1,"Make the changes permanent");
		if (state!=MS_LB_UP) return 1;

		stat_points_used=0;

		for (n=0; n<108; n++) 
		{
			if (stat_raised[n]) 
			{
				if (n>7) 
				{
					m=skilltab[n-8].nr+8;
				} 
				else 
					m=n;
				cmd(CL_CMD_STAT,m,stat_raised[n]);
			}
			stat_raised[n]=0;
		}
		return 1;
	}

	if (x<gui_skl_pm[RECT_X1]) return 0;
	if (x>gui_skl_pm[RECT_X2]) return 0;
	if (y<gui_skl_pm[RECT_Y1]) return 0;
	if (y>gui_skl_pm[RECT_Y2]) return 0;

	n=(y-2)/14;

	hightlight=HL_STATBOX;
	hightlight_sub=n;

	if (x<172) { // raise
		if (state==MS_RB_UP) {
			if (n<5) xlog(1,"Raise %s.",at_name[n]);
			else if (n==5) xlog(1,"Raise Hitpoints.");
			else if (n==6) xlog(1,"Raise Mana."); 	// xlog(1,"Raise Endurance.");
			else if (n==7) return 1; 				// xlog(1,"Raise Mana.");
			else xlog(1,"Raise %s.",skilltab[n-8+skill_pos].name);
			return 1;
		}
		if (state!=MS_LB_UP) return 1;

		if (n<5) {
			if (attrib_needed(n,pl.attrib[n][0]+stat_raised[n])>pl.points-stat_points_used) return 1;
			stat_points_used+=attrib_needed(n,pl.attrib[n][0]+stat_raised[n]);
			stat_raised[n]++;
			return 1;
		} else if (n==5) {
			if (hp_needed(pl.hp[0]+stat_raised[n])>pl.points-stat_points_used) return 1;
			stat_points_used+=hp_needed(pl.hp[0]+stat_raised[n]);
			stat_raised[n]++;
			return 1;
		} 
		/*
		else if (n==6) 
		{
			if (end_needed(pl.end[0]+stat_raised[n])>pl.points-stat_points_used) return 1;
			stat_points_used+=end_needed(pl.end[0]+stat_raised[n]);
			stat_raised[n]++;
			return 1;
		} 
		*/
		else if (n==6) 
		{
			if (mana_needed(pl.mana[0]+stat_raised[n+1])>pl.points-stat_points_used) return 1;
			stat_points_used+=mana_needed(pl.mana[0]+stat_raised[n+1]);
			stat_raised[n+1]++;
			return 1;
		} 
		else if (n==7) return 1;
		else {
			m=skilltab[n-8+skill_pos].nr;
			if (skill_needed(m,pl.skill[m][0]+stat_raised[n+skill_pos])>pl.points-stat_points_used) return 1;
			stat_points_used+=skill_needed(m,pl.skill[m][0]+stat_raised[n+skill_pos]);
			stat_raised[n+skill_pos]++;
			return 1;
		}
	} 
	else 
	{ // lower
		if (state==MS_RB_UP) {
			if (n<5) xlog(1,"Lower %s.",at_name[n]);
			else if (n==5) xlog(1,"Lower Hitpoints.");
			else if (n==6) xlog(1,"Lower Mana."); 	// xlog(1,"Lower Endurance.");
			else if (n==7) return 1; 				// xlog(1,"Lower Mana.");
			else xlog(1,"Lower %s.",skilltab[n-8+skill_pos].name);
			return 1;
		}
		if (state!=MS_LB_UP) return 1;

		if (n<5) {
			if (!stat_raised[n]) return 1;
			stat_raised[n]--;
			stat_points_used-=attrib_needed(n,pl.attrib[n][0]+stat_raised[n]);
			return 1;
		} else if (n==5) {
			if (!stat_raised[n]) return 1;
			stat_raised[n]--;
			stat_points_used-=hp_needed(pl.hp[0]+stat_raised[n]);
			return 1;
		} 
		/*
		else if (n==6) 
		{
			if (!stat_raised[n]) return 1;
			stat_raised[n]--;
			stat_points_used-=end_needed(pl.end[0]+stat_raised[n]);
			return 1;
		} 
		*/
		else if (n==6) 
		{
			if (!stat_raised[n+1]) return 1;
			stat_raised[n+1]--;
			stat_points_used-=mana_needed(pl.mana[0]+stat_raised[n+1]);
			return 1;
		} 
		else if (n==7) return 1;
		else {
			if (!stat_raised[n+skill_pos]) return 1;
			m=skilltab[n-8+skill_pos].nr;
			stat_raised[n+skill_pos]--;
			stat_points_used-=skill_needed(m,pl.skill[m][0]+stat_raised[n+skill_pos]);
			return 1;
		}
	}
}

int mouse_statbox(int x,int y,int state)
{
    int n,m,keys,ret;

    keys=0;
	if (GetAsyncKeyState(VK_SHIFT)&0x8000) keys|=1;
	if (GetAsyncKeyState(VK_CONTROL)&0x8000) keys|=2;

    if (state==MS_LB_UP) 
	{
       if (keys&2) m=90;
       else if (keys&1) m=10;
       else m=1;
    } 
	else 
		m=1;

    for (n=0; n<m; n++) ret=_mouse_statbox(x,y,state);

    return ret;
}

int mouse_statbox2(int x,int y,int state)
{
	int n;
    extern struct skilltab _skilltab[];

	/*
	if (screen_windowed == 1) {
        y-=1;
    }
	else
	{
		y+=1;
	}
	*/
	
	// Attributes
	if (state==MS_RB_UP) 
	{
		int xt=5,yt=5,xb=136,yb=18,shf=14;
		
		// 5,  5		136, 18
		// 5, 19		136, 32
		// 5, 33		136, 46
		
		// Braveness
		n=0;
		if ( x>xt && y>yt+shf*n && x<xb && y<yb+shf*n )
		{
			xlog(1,"%s improves most skills and spells. It also improves the speed of casting spells and using skills.",at_name[n]);
			last_skill = 50;
			return 1;
		}
		// Willpower
		n=1;
		if ( x>xt && y>yt+shf*n && x<xb && y<yb+shf*n )
		{
			xlog(1,"%s improves most support spells. Improves support spells cast on players with lower aptitude.",at_name[n]);
			last_skill = 51;
			return 1;
		}
		// Intuition
		n=2;
		if ( x>xt && y>yt+shf*n && x<xb && y<yb+shf*n )
		{
			xlog(1,"%s improves most offensive spells. It also reduces the duration of skill exhaustion.",at_name[n]);
			last_skill = 52;
			return 1;
		}
		// Agility
		n=3;
		if ( x>xt && y>yt+shf*n && x<xb && y<yb+shf*n )
		{
			xlog(1,"%s improves most combat skills. It also improves your movement speed and your attack speed.",at_name[n]);
			last_skill = 53;
			return 1;
		}
		// Strength
		n=4;
		if ( x>xt && y>yt+shf*n && x<xb && y<yb+shf*n )
		{
			xlog(1,"%s improves most combat skills. It also improves your movement speed and the damage dealt by your attacks.",at_name[n]);
			last_skill = 54;
			return 1;
		}

	}
	
	if (x<gui_skl_names[RECT_X1]) return 0;
	if (x>gui_skl_names[RECT_X2]) return 0;
	if (y<gui_skl_names[RECT_Y1]) return 0;
	if (y>gui_skl_names[RECT_Y2]) return 0;

	n=(y-114)/14;

	hightlight=HL_STATBOX2;
	hightlight_sub=n;
	
	// Skills
	if (state==MS_RB_UP) 
	{
		if (pl.skill[skilltab[n+skill_pos].nr][0]) 
		{
			xlog(1,skilltab[n+skill_pos].desc);
			if (last_skill == n+skill_pos)
			{
				last_skill = -1;
			}
			else
			{
				last_skill = n+skill_pos;
			}
		}
	} 
	else if (state==MS_LB_UP) 
	{
		cmd3(CL_CMD_SKILL,skilltab[n+skill_pos].nr,selected_char,skilltab[n+skill_pos].attrib[0]);
	}
	return 1;
}

void cmd(int cmd,int x,int y)
{
	unsigned char buf[16];

	play_sound("sfx\\click.wav",-1000,0);

	buf[0]=(char)cmd;
	*(unsigned short*)(buf+1)=(short)x;
	*(unsigned long*)(buf+3)=(long)y;
	xsend(buf);
}

void cmds(int cmd,int x,int y)
{
	unsigned char buf[16];

	buf[0]=(char)cmd;
	*(unsigned short*)(buf+1)=(short)x;
	*(unsigned long*)(buf+3)=(long)y;
	xsend(buf);
}

void cmd3(int cmd,int x,int y,int z)
{
	unsigned char buf[16];

	play_sound("sfx\\click.wav",-1000,0);

	buf[0]=(char)cmd;
	*(unsigned long*)(buf+1)=x;
	*(unsigned long*)(buf+5)=y;
	*(unsigned long*)(buf+9)=z;
	xsend(buf);
}

void cmd1(int cmd,int x)
{
	unsigned char buf[16];

	play_sound("sfx\\click.wav",-1000,0);

	buf[0]=(char)cmd;
	*(unsigned int*)(buf+1)=x;
	xsend(buf);
}

void cmd1s(int cmd,int x)
{
	unsigned char buf[16];

	buf[0]=(char)cmd;
	*(unsigned int*)(buf+1)=x;
	xsend(buf);
}

void mouse_mapbox(int x,int y,int state)
{
	int mx,my,m,keys,dist_diff;
	int xr,yr,tst;

	// X and Y without offset below
	xr=x;
	yr=y;

	x+=176-16;
	y+=8;

	if (screen_windowed == 1) {
        y+=4;
    }

	dist_diff=(screen_renderdist-screen_viewsize)/2;

	mx=2*y+x-(screen_tileyoff*2)-screen_tilexoff+((screen_renderdist-34)/2*32);
	my=x-2*y+(screen_tileyoff*2)-screen_tilexoff+((screen_renderdist-34)/2*32);

        if (mx<(dist_diff+3)*32+12 || mx>(screen_renderdist-dist_diff-view_subedges-3)*32+20 || my<(dist_diff+view_subedges+3)*32+12 || my>(screen_renderdist-dist_diff-3)*32+20) {
			// Clicking auto-walk buttons
			if (state==MS_LB_UP) 
			{
				if 		((int)sqrt(pow(xr-xwalk_nx,2)+pow(yr-xwalk_ny,2)) < 12)	xmove=1; // North
				else if ((int)sqrt(pow(xr-xwalk_wx,2)+pow(yr-xwalk_wy,2)) < 12)	xmove=2; // West
				else if ((int)sqrt(pow(xr-xwalk_sx,2)+pow(yr-xwalk_sy,2)) < 12)	xmove=3; // South
				else if ((int)sqrt(pow(xr-xwalk_ex,2)+pow(yr-xwalk_ey,2)) < 12)	xmove=4; // East
				
				/*mx/=32; my/=32;
			
				if (my==screen_viewsize/2 && (mx==2 || mx==3)) xmove=1;
				if (mx==screen_viewsize/2 && (my==6 || my==7)) xmove=2;
				if (my==screen_viewsize/2 && (mx==27 || mx==28)) xmove=3;
				if (mx==screen_viewsize/2 && (my==31 || my==32)) xmove=4;*/
			}
			return;
		}

	mx=mx/32;
	my=my/32;

        tile_x=-1; tile_y=-1; tile_type=-1;
	if (mx<dist_diff+3 || mx>screen_renderdist-dist_diff-view_subedges-3) return;
	if (my<dist_diff+view_subedges+3 || my>screen_renderdist-dist_diff-3) return;

	m=mx+my*screen_renderdist;

//	xlog("light=%d",map[m].light);
//  xlog("ch_sprite=%d, nr=%d, id=%d",map[m].ch_sprite,map[m].ch_nr,map[m].ch_id);

	if (pl.citem==46) { // build
		if (state==MS_RB_UP) { xmove=0; cmd(CL_CMD_DROP,map[m].x,map[m].y); }
		if (state==MS_LB_UP) { xmove=0; cmd(CL_CMD_PICKUP,map[m].x,map[m].y); }
		tile_type=0; tile_x=mx; tile_y=my;
		hightlight=HL_MAP;
		return;
	}

	keys=0;
	if (GetAsyncKeyState(VK_SHIFT)&0x8000) keys|=1;
	if (GetAsyncKeyState(VK_CONTROL)&0x8000) keys|=2;
	if (GetAsyncKeyState(VK_MENU)&0x8000) keys|=4;

	if (keys==0) {
		tile_x=mx; tile_y=my;
		tile_type=0;

		if (state==MS_RB_UP) { xmove=0; cmd(CL_CMD_TURN,map[m].x,map[m].y); }
		if (state==MS_LB_UP) { xmove=0; cmd(CL_CMD_MOVE,map[m].x,map[m].y); }
		hightlight=HL_MAP;
		cursor_type=CT_WALK;
		return;
	}

	if (keys==1) {
		if (pl.citem) { hightlight=HL_MAP; cursor_type=CT_DROP; }
		else if (map[m].flags&ISITEM) ;
		else if (map[m+1-screen_renderdist].flags&ISITEM) { mx++; my--; }
		else if (map[m+2-2*screen_renderdist].flags&ISITEM) { mx+=2; my-=2; }
		else if (map[m+1].flags&ISITEM) { mx++; }
		else if (map[m+screen_renderdist].flags&ISITEM) { my++; }
		else if (map[m-1].flags&ISITEM) { mx--; }
		else if (map[m-screen_renderdist].flags&ISITEM) { my--; }
		else if (map[m+1+screen_renderdist].flags&ISITEM) { mx++; my++; }
		else if (map[m-1+screen_renderdist].flags&ISITEM) { mx--; my++; }
		else if (map[m-1-screen_renderdist].flags&ISITEM) { mx--; my--; }
		else if (map[m+2].flags&ISITEM) { mx+=2; }
		else if (map[m+2*screen_renderdist].flags&ISITEM) { my+=2; }
		else if (map[m-2].flags&ISITEM) { mx-=2; }
		else if (map[m-2*screen_renderdist].flags&ISITEM) { my-=2; }
		else if (map[m+1+2*screen_renderdist].flags&ISITEM) { mx++; my+=2; }
		else if (map[m-1+2*screen_renderdist].flags&ISITEM) { mx--; my+=2; }
		else if (map[m+1-2*screen_renderdist].flags&ISITEM) { mx++; my-=2; }
		else if (map[m-1-2*screen_renderdist].flags&ISITEM) { mx--; my-=2; }
		else if (map[m+2+1*screen_renderdist].flags&ISITEM) { mx+=2; my++; }
		else if (map[m-2+1*screen_renderdist].flags&ISITEM) { mx-=2; my++; }
		else if (map[m+2-1*screen_renderdist].flags&ISITEM) { mx+=2; my--; }
		else if (map[m-2-1*screen_renderdist].flags&ISITEM) { mx-=2; my--; }
		else if (map[m+2+2*screen_renderdist].flags&ISITEM) { mx+=2; my+=2; }
		else if (map[m-2+2*screen_renderdist].flags&ISITEM) { mx-=2; my+=2; }
		else if (map[m-2-2*screen_renderdist].flags&ISITEM) { mx-=2; my-=2; }

		m=mx+my*screen_renderdist;
		tile_x=mx; tile_y=my;

		if (pl.citem && map[m].flags&ISITEM) { if (map[m].flags&ISUSABLE) cursor_type=CT_USE; else cursor_type=CT_NONE; }
		else if (map[m].flags&ISITEM) { hightlight=HL_MAP; if (map[m].flags&ISUSABLE) cursor_type=CT_USE; else cursor_type=CT_TAKE; }

		if (pl.citem && !(map[m].flags&ISITEM)) {
			if (state==MS_LB_UP) { xmove=0; cmd(CL_CMD_DROP,map[m].x,map[m].y); }
			tile_type=0;
		}
		if ((map[m].flags&ISITEM)) {
			if (state==MS_LB_UP) {
				if (map[m].flags&ISUSABLE) { xmove=0; cmd(CL_CMD_USE,map[m].x,map[m].y); noshop=0; }
				else { xmove=0; cmd(CL_CMD_PICKUP,map[m].x,map[m].y); }
			}
			if (state==MS_RB_UP) { xmove=0; cmd(CL_CMD_LOOK_ITEM,map[m].x,map[m].y); }
			tile_type=1;
		}
		return;
	}

	if (keys==2) {
		if (map[m].flags&ISCHAR) hightlight=HL_MAP;
		else if (map[m+1-screen_renderdist].flags&ISCHAR) { mx++; my--; hightlight=HL_MAP; }
		else if (map[m+2-2*screen_renderdist].flags&ISCHAR) { mx+=2; my-=2; hightlight=HL_MAP; }
		else if (map[m+1].flags&ISCHAR) { mx++; hightlight=HL_MAP; }
		else if (map[m+screen_renderdist].flags&ISCHAR) { my++; hightlight=HL_MAP; }
		else if (map[m-1].flags&ISCHAR) { mx--; hightlight=HL_MAP; }
		else if (map[m-screen_renderdist].flags&ISCHAR) { my--; hightlight=HL_MAP; }
		else if (map[m+1+screen_renderdist].flags&ISCHAR) { mx++; my++; hightlight=HL_MAP; }
		else if (map[m-1+screen_renderdist].flags&ISCHAR) { mx--; my++; hightlight=HL_MAP; }
		else if (map[m-1-screen_renderdist].flags&ISCHAR) { mx--; my--; hightlight=HL_MAP; }
		else if (map[m+2].flags&ISCHAR) { mx+=2; hightlight=HL_MAP; }
		else if (map[m+2*screen_renderdist].flags&ISCHAR) { my+=2; hightlight=HL_MAP; }
		else if (map[m-2].flags&ISCHAR) { mx-=2; hightlight=HL_MAP; }
		else if (map[m-2*screen_renderdist].flags&ISCHAR) { my-=2; hightlight=HL_MAP; }
		else if (map[m+1+2*screen_renderdist].flags&ISCHAR) { mx++; my+=2; hightlight=HL_MAP; }
		else if (map[m-1+2*screen_renderdist].flags&ISCHAR) { mx--; my+=2; hightlight=HL_MAP; }
		else if (map[m+1-2*screen_renderdist].flags&ISCHAR) { mx++; my-=2; hightlight=HL_MAP; }
		else if (map[m-1-2*screen_renderdist].flags&ISCHAR) { mx--; my-=2; hightlight=HL_MAP; }
		else if (map[m+2+1*screen_renderdist].flags&ISCHAR) { mx+=2; my++; hightlight=HL_MAP; }
		else if (map[m-2+1*screen_renderdist].flags&ISCHAR) { mx-=2; my++; hightlight=HL_MAP; }
		else if (map[m+2-1*screen_renderdist].flags&ISCHAR) { mx+=2; my--; hightlight=HL_MAP; }
		else if (map[m-2-1*screen_renderdist].flags&ISCHAR) { mx-=2; my--; hightlight=HL_MAP; }
		else if (map[m+2+2*screen_renderdist].flags&ISCHAR) { mx+=2; my+=2; hightlight=HL_MAP; }
		else if (map[m-2+2*screen_renderdist].flags&ISCHAR) { mx-=2; my+=2; hightlight=HL_MAP; }
		else if (map[m-2-2*screen_renderdist].flags&ISCHAR) { mx-=2; my-=2; hightlight=HL_MAP; }

		m=mx+my*screen_renderdist;
		tile_x=mx; tile_y=my;

      if (map[m].flags&ISCHAR) {
			if (pl.citem) cursor_type=CT_GIVE;
			else cursor_type=CT_HIT;
		}

		if (map[m].flags&ISCHAR) {
			if (pl.citem && state==MS_LB_UP) { xmove=0; cmd1(CL_CMD_GIVE,map[m].ch_nr); }
			else if (state==MS_LB_UP) { xmove=0; cmd1(CL_CMD_ATTACK,map[m].ch_nr); }
			else if (state==MS_RB_UP) { xmove=0; cmd1(CL_CMD_LOOK,map[m].ch_nr); noshop=0; }
			tile_type=2;
		}
	}

	if (keys==4) {
		if (map[m].flags&ISCHAR) hightlight=HL_MAP;
		else if (map[m+1-screen_renderdist].flags&ISCHAR) { mx++; my--; hightlight=HL_MAP; }
		else if (map[m+2-2*screen_renderdist].flags&ISCHAR) { mx+=2; my-=2; hightlight=HL_MAP; }
		else if (map[m+1].flags&ISCHAR) { mx++; hightlight=HL_MAP; }
		else if (map[m+screen_renderdist].flags&ISCHAR) { my++; hightlight=HL_MAP; }
		else if (map[m-1].flags&ISCHAR) { mx--; hightlight=HL_MAP; }
		else if (map[m-screen_renderdist].flags&ISCHAR) { my--; hightlight=HL_MAP; }
		else if (map[m+1+screen_renderdist].flags&ISCHAR) { mx++; my++; hightlight=HL_MAP; }
		else if (map[m-1+screen_renderdist].flags&ISCHAR) { mx--; my++; hightlight=HL_MAP; }
		else if (map[m-1-screen_renderdist].flags&ISCHAR) { mx--; my--; hightlight=HL_MAP; }
		else if (map[m+2].flags&ISCHAR) { mx+=2; hightlight=HL_MAP; }
		else if (map[m+2*screen_renderdist].flags&ISCHAR) { my+=2; hightlight=HL_MAP; }
		else if (map[m-2].flags&ISCHAR) { mx-=2; hightlight=HL_MAP; }
		else if (map[m-2*screen_renderdist].flags&ISCHAR) { my-=2; hightlight=HL_MAP; }
		else if (map[m+1+2*screen_renderdist].flags&ISCHAR) { mx++; my+=2; hightlight=HL_MAP; }
		else if (map[m-1+2*screen_renderdist].flags&ISCHAR) { mx--; my+=2; hightlight=HL_MAP; }
		else if (map[m+1-2*screen_renderdist].flags&ISCHAR) { mx++; my-=2; hightlight=HL_MAP; }
		else if (map[m-1-2*screen_renderdist].flags&ISCHAR) { mx--; my-=2; hightlight=HL_MAP; }
		else if (map[m+2+1*screen_renderdist].flags&ISCHAR) { mx+=2; my++; hightlight=HL_MAP; }
		else if (map[m-2+1*screen_renderdist].flags&ISCHAR) { mx-=2; my++; hightlight=HL_MAP; }
		else if (map[m+2-1*screen_renderdist].flags&ISCHAR) { mx+=2; my--; hightlight=HL_MAP; }
		else if (map[m-2-1*screen_renderdist].flags&ISCHAR) { mx-=2; my--; hightlight=HL_MAP; }
		else if (map[m+2+2*screen_renderdist].flags&ISCHAR) { mx+=2; my+=2; hightlight=HL_MAP; }
		else if (map[m-2+2*screen_renderdist].flags&ISCHAR) { mx-=2; my+=2; hightlight=HL_MAP; }
		else if (map[m-2-2*screen_renderdist].flags&ISCHAR) { mx-=2; my-=2; hightlight=HL_MAP; }

		m=mx+my*screen_renderdist;
		tile_x=mx; tile_y=my;

		if (map[m].flags&ISCHAR) {
			cursor_type=CT_SEL;
			if (state==MS_LB_UP) { xmove=0; if (selected_char==map[m].ch_nr) selected_char=0; else selected_char=map[m].ch_nr; }
			else if (state==MS_RB_UP) { xmove=0; cmd1(CL_CMD_LOOK,map[m].ch_nr); noshop=0; }
			tile_type=2;
		} else if (state==MS_LB_UP) selected_char=0;
	}
}

#define GUI_SHOP_X		((1280/2)-(320/2))
#define GUI_SHOP_Y		((736/2)-(320/2)+72)

int mouse_shop(int x,int y,int mode)
{
	int nr;
	int tx,ty;

	if (!show_shop) return 0;

	if (x>(GUI_SHOP_X+279) && x<(GUI_SHOP_X+296) && y>(GUI_SHOP_Y) && y<(GUI_SHOP_Y+14)) 
	{
		if (mode==MS_LB_UP) 
		{ 
			show_shop=0; 
			noshop=QSIZE*36; 
		}
		return 1;
	}

	if (x>(GUI_SHOP_X) && x<(GUI_SHOP_X+280) && y>(GUI_SHOP_Y+1) && y<(GUI_SHOP_Y+1+292)) 
	{
		tx=(x-(GUI_SHOP_X  ))/35;
		ty=(y-(GUI_SHOP_Y+1))/35;

		nr=tx+ty*8;
		if (mode==MS_LB_UP) cmd(CL_CMD_SHOP,shop.nr,nr);
		if (mode==MS_RB_UP) cmd(CL_CMD_SHOP,shop.nr,nr+62);

		if (shop.item[nr])	{ cursor_type=CT_TAKE; }
		else if (pl.citem)	{ cursor_type=CT_DROP; }
		hightlight=HL_SHOP;
		hightlight_sub=nr;
		return 1;
	}
	return 0;
}


void mouse(int x,int y,int state)
{
	if (!init_done) return;

	hightlight=0;
	cursor_type=CT_NONE;

	if (screen_windowed == 1) {
		// Adjust position when windowed
		//y += 4;
		x += 2;
		y += 1;
	}
	else
	{
		y -= 2;
	}

	mouse_x=x; mouse_y=y;
	if (mouse_inventory(x,y,state)) ;
	else if (mouse_shop(x,y,state)) ;
	else if (mouse_buttonbox(x,y,state)) ;
	else if (mouse_statbox(x,y,state)) ;
	else if (mouse_statbox2(x,y,state)) ;
	else mouse_mapbox(x,y,state);
}

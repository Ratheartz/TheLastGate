/*************************************************************************

   This file is part of 'Mercenaries of Astonia v2'
   Copyright (c) 1997-2001 Daniel Brockhaus (joker@astonia.com)
   All rights reserved.

 **************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "server.h"

int diffi = 21;

int build_item(int nr, int x, int y)
{
	int in, n, m;

	if (x<0 || x>=MAPX || y<0 || y>=MAPY)
	{
		return 0;
	}

	if (map[x + y * MAPX].it)
	{
		return 0;
	}

	xlog("build: add item %d at %d,%d", nr, x, y);

	if (map[x + y * MAPX].fsprite)
	{
		map[x + y * MAPX].fsprite = 0;
	}

	if (it_temp[nr].flags & (IF_TAKE | IF_LOOK | IF_LOOKSPECIAL | IF_USE | IF_USESPECIAL))
	{
		in = god_create_item(nr);

		if (it[in].driver==33)
		{
			for (n = 1, m = 0; n<MAXITEM; n++)
			{
				if (it[n].used==USE_EMPTY)
				{
					continue;
				}
				if (it[n].driver==33)
				{
					m = max(it[n].data[0], m);
				}
			}

			m++;
			do_area_log(0, 0, x, y, 1, "Respawn point %d, Level %d.\n", m, diffi);
			it[in].data[0] = m;
			it[in].data[9] = diffi;   // difficulty level
			it[in].temp = 0; // make sure it isn't reset when doing #reset
		}

		if (it[in].driver==94)
		{
			do_area_log(0, 0, x, y, 1, "Exit portal, Reward %d.\n", diffi);
			it[in].data[9] = diffi;   // difficulty level
			it[in].temp = 0; // make sure it isn't reset when doing #reset
		}

		map[x + y * MAPX].it = in;

		it[in].x = (short)x;
		it[in].y = (short)y;
		it[in].carried = 0;
	}
	else
	{
		map[x + y * MAPX].fsprite = it_temp[nr].sprite[0];
		if (it_temp[nr].flags & IF_MOVEBLOCK)
		{
			map[x + y * MAPX].flags |= MF_MOVEBLOCK;
		}
		if (it_temp[nr].flags & IF_SIGHTBLOCK)
		{
			map[x + y * MAPX].flags |= MF_SIGHTBLOCK;
		}
		return 0;
	}

	return(in);
}

void build_drop(int x, int y, int in)
{
	int nr, exfloor = 0;
	static int trees1[15] = {19, 19, 19, 20, 20, 20, 48, 48, 48, 49, 49, 49, 606, 607, 607};
	static int trees2[15] = {19, 19, 20, 20, 48, 48, 49, 49, 606, 607, 607, 608, 608, 609, 609};
	
	static int trees3[15] = {1680, 1680, 1680, 1681, 1681, 
							 1681, 1682, 1682, 1682, 1683, 
							 1683, 1683, 1684, 1685, 1685};
	static int trees4[15] = {1680, 1680, 1681, 1681, 1682, 
							 1682, 1683, 1683, 1684, 1685, 
							 1685, 1686, 1686, 1687, 1687};
							 
	static int trees5[5] = {200, 236, 237, 238, 239};

	if (x<0 || x>=MAPX || y<0 || y>=MAPY)
	{
		return;
	}

	if (in & 0x40000000)
	{
		map[x + y * MAPX].flags ^= in & 0xfffffff;
		return;
	}
	if (in & 0x20000000)
	{
		nr = in & 0xfffffff;
		if (nr==520 && !RANDOM(4))	// hack for red carpet
		{
			nr += RANDOM(3);
		}
		if (nr==531 && !RANDOM(4))	// hack for purple carpet
		{
			nr += RANDOM(3);
		}
		if (nr==5034 && !RANDOM(4))	// hack for green carpet
		{
			nr += RANDOM(3);
		}
		if (nr==1003)   // hack for jungle ground
		{
			nr += RANDOM(4);
			if (nr>1003)
			{
				nr++;
			}
		}
		if (nr==542)   // hack for street ground
		{
			nr += RANDOM(9);
		}
		if (nr==551 || nr==2823 || nr==2828)   // hack for grass grounds
		{
			nr += RANDOM(4);
		}
		if (nr==133)   // hack for parkett ground
		{
			nr += RANDOM(9);
		}
		if (nr==142)   // hack for parkett ground
		{
			nr += RANDOM(4);
		}
		if (nr==170)   // hack for stone ground
		{
			nr += RANDOM(6);
		}
		if (nr==808)	// hack for greystone ground
		{
			nr += RANDOM(4);
		}
		if (nr==704)
		{
			static int tab[4] = {704, 659, 660, 688};
			nr = tab[RANDOM(4)];
		}
		if (nr==950)
		{
			nr += RANDOM(9);
		}
		if (nr==959)
		{
			nr += (x % 3) * 3 + (y % 3);
		}
		if (nr==16670)
		{
			nr += RANDOM(4);
		}
		if (nr==16933)
		{
			static int tab[4] = {16933, 16957, 16958, 16959};
			nr = tab[RANDOM(4)];
		}
		if (nr==662)	// hack for garg nest floor
		{
			// Death Red, light red, dark red, brick
			static int tab[4] = {1099, 1109, 1014, 558};
			nr = abs((x-y)%4);
			nr = tab[nr];
		}
		if (nr==16689)	// hack for icey nest floor
		{
			// Marble, Plain Grey, Blind Grey, Purple-grey
			static int tab[4] = {1158, 1100, 1010, 1141};
			nr = abs((x-y)%4);
			nr = tab[nr];
		}

		map[x + y * MAPX].sprite = nr;
		return;
	}
	
	switch (in)
	{
		case 1:		// special cleaning material
			map[x + y * MAPX].flags &= ~(MF_MOVEBLOCK | MF_SIGHTBLOCK); 
			break;
		case 1467:	// hack for random trees - town
			in = trees1[RANDOM(15)];
			break;
		case 1468:	// hack for random trees - any
			in = trees2[RANDOM(15)];
			break;
		case 1695:	// hack for random autumn trees - small
			in = trees3[RANDOM(15)];
			break;
		case 1696:	// hack for random autumn trees - large
			in = trees4[RANDOM(15)];
			break;
		case 1902:	// hack for random jungle trees
			in = trees5[RANDOM(5)];
			break;
		
		case 1936: exfloor = 0x20000000 | 1002; 	break;
		case 1937: exfloor = 0x20000000 | 1008; 	break;
		case 1938: exfloor = 0x20000000 | 1013; 	break;
		case 1939: exfloor = 0x20000000 | 1034; 	break;
		case 1940: exfloor = 0x20000000 | 1010; 	break;
		case 1941: exfloor = 0x20000000 | 1052; 	break;
		case 1942: exfloor = 0x20000000 | 1100; 	break;
		case 1943: exfloor = 0x20000000 | 1099; 	break;
		case 1944: exfloor = 0x20000000 | 1012; 	break;
		case 1945: exfloor = 0x20000000 | 1109; 	break;
		case 1946: exfloor = 0x20000000 | 1118; 	break;
		case 1947: exfloor = 0x20000000 | 1141; 	break;
		case 1948: exfloor = 0x20000000 | 1158; 	break;
		case 1949: exfloor = 0x20000000 | 1145; 	break;
		case 1950: exfloor = 0x20000000 | 1014; 	break;
		case 1951: exfloor = 0x20000000 | 1005; 	break;
		case 1952: exfloor = 0x20000000 | 1006; 	break;
		case 1953: exfloor = 0x20000000 | 1007; 	break;
		case 1954: exfloor = 0x20000000 |  402; 	break;
		case 1955: exfloor = 0x20000000 |  500; 	break;
		
		default: 
			break;
	}
	
	if (exfloor)
	{
		map[x + y * MAPX].sprite = exfloor;
	}
	
	nr = build_item(in, x, y);
	if (!nr)
	{
		return;
	}

	if (it[nr].active)
	{
		if (it[nr].light[1])
		{
			do_add_light(x, y, it[nr].light[1]);
		}
	}
	else
	{
		if (it[nr].light[0])
		{
			do_add_light(x, y, it[nr].light[0]);
		}
	}
}

void build_remove(int x, int y)
{
	int nr;

	if (x<0 || x>=MAPX || y<0 || y>=MAPY)
	{
		return;
	}

	map[x + y * MAPX].fsprite = 0;
	map[x + y * MAPX].flags &= ~(MF_MOVEBLOCK | MF_SIGHTBLOCK);

	nr = map[x + y * MAPX].it;
	if (!nr)
	{
		return;
	}

	if (it[nr].active)
	{
		if (it[nr].light[1])
		{
			do_add_light(x, y, -it[nr].light[1]);
		}
	}
	else
	{
		if (it[nr].light[0])
		{
			do_add_light(x, y, -it[nr].light[0]);
		}
	}

	it[nr].used = 0;
	map[x + y * MAPX].it = 0;

	xlog("build: remove item from %d,%d", x, y);
}

// Copy from X/Y to X/Y
int build_copy(int fx, int fy, int tx, int ty)
{
	int nr, in, act=0;

	if (fx<0 || fx>=MAPX || fy<0 || fy>=MAPY || tx<0 || tx>=MAPX || ty<0 || ty>=MAPY)
	{
		xlog("build_copy: location out of bounds");
		return 0;
	}
	
	// Clean tile
	build_remove(tx, ty);
	
	// Copy floor
	map[tx + ty * MAPX].sprite = map[fx + fy * MAPX].sprite;
	
	// Copy object
	nr = map[fx + fy * MAPX].it;
	if (nr)
	{
		in = it[nr].temp;
		act= it[nr].active;
		nr = build_item(in, tx, ty);
	}
	
	// Copy flags
	map[tx + ty * MAPX].flags = map[fx + fy * MAPX].flags;
	
	if (nr)
	{
		it[nr].active = act;
		if (it[nr].active)
		{
			if (it[nr].light[1])
			{
				do_add_light(tx, ty, it[nr].light[1]);
			}
		}
		else
		{
			if (it[nr].light[0])
			{
				do_add_light(tx, ty, it[nr].light[0]);
			}
		}
	}
	
	return 1;
}

int build_copy_rect(int fx, int fy, int tx, int ty, int w, int h)
{
	int x, y, try=1;
	
	if (fx<0 || fx+w>=MAPX || fy<0 || fy+h>=MAPY || tx<0 || tx+w>=MAPX || ty<0 || ty+h>=MAPY)
	{
		xlog("build_copy_rect: location out of bounds");
		return 0;
	}
	
	for (x=0; x<w; x++)
	{
		for (y=0; y<h; y++)
		{
			try = build_copy(fx+x, fy+y, tx+x, ty+y);
			if (!try)
			{
				return 0;
			}
		}
	}
	return 1;
}

void build_clean_lights(int fx, int fy, int tx, int ty)
{
	int x, y, in, m, cnt1 = 0, cnt2 = 0;
	
	if (fx<0 || fx>=MAPX || fy<0 || fy>=MAPY || tx<0 || tx>=MAPX || ty<0 || ty>=MAPY)
	{
		xlog("build_clean_lights: location out of bounds");
		return;
	}
	
	for (y = fy; y<=ty; y++)
	{
		for (x = fx; x<=tx; x++)
		{
			m = XY2M(x, y);
			map[m].light  = 0;
			map[m].dlight = 0;
		}
	}

	for (y = fy; y<=ty; y++)
	{
		printf("%4d/%4d (%d,%d)\r", y, MAPY, cnt1, cnt2);
		fflush(stdout);
		for (x = fx; x<=tx; x++)
		{
			m = XY2M(x, y);
			if (map[m].flags & MF_INDOORS)
			{
				cnt2++;
				compute_dlight(x, y);
			}
			if ((in = map[m].it)==0)
			{
				continue;
			}
			if (it[in].active)
			{
				if (it[in].light[1])
				{
					do_add_light(x, y, it[in].light[1]);
					cnt1++;
				}
			}
			else
			{
				if (it[in].light[0])
				{
					do_add_light(x, y, it[in].light[0]);
					cnt1++;
				}
			}
		}
	}
}

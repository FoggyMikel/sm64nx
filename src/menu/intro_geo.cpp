#include "ultra64.h"
#include "sm64.h"
#include "prevent_bss_reordering.h"
#include "types.h"
#include "game/memory.h"
#include "game/segment2.h"
#include "game/segment7.h"
#include "intro_geo.h"

#include "gfx_dimensions.h"
#include "game/segment7.h"
#include "levels/intro/header.h"
#include "textures.h"

// frame counts for the zoom in, hold, and zoom out of title model
#define INTRO_STEPS_ZOOM_IN (20 * FRAME_RATE_SCALER_INV)
#define INTRO_STEPS_HOLD_1 (75 * FRAME_RATE_SCALER_INV)
#define INTRO_STEPS_ZOOM_OUT (91 * FRAME_RATE_SCALER_INV)

// background types
#define INTRO_BACKGROUND_SUPER_MARIO 0
#define INTRO_BACKGROUND_GAME_OVER 1

struct GraphNodeMore
{
	/*0x00*/ struct GraphNode node;
	/*0x14*/ void* todo;
	/*0x18*/ u32 unk18;
};
/*
// title screen segment A
extern Gfx title_screen_bg_dl_0A000118[];
extern Gfx title_screen_bg_dl_0A000100[];
extern Gfx title_screen_bg_dl_0A000130[];
extern Gfx title_screen_bg_dl_0A000148[];
extern Gfx title_screen_bg_dl_0A000160[];
extern Gfx title_screen_bg_dl_0A000178[];
extern Gfx title_screen_bg_dl_0A000190[];
extern const u8 *const mario_title_texture_table[];
extern const u8 *const game_over_texture_table[];
*/

// intro geo bss
int gGameOverFrameCounter;
int gGameOverTableIndex;
s16 gTitleZoomCounter;
s32 gTitleFadeCounter;

// intro screen background display lists for each of four 80x20 textures
const Gfx* introBackgroundDlRows[] = {title_screen_bg_dl_0A000130, title_screen_bg_dl_0A000148, title_screen_bg_dl_0A000160, title_screen_bg_dl_0A000178};

// intro screen background texture X offsets
float introBackgroundOffsetX[] = {
    0.0, 80.0, 160.0, 240.0, 0.0, 80.0, 160.0, 240.0, 0.0, 80.0, 160.0, 240.0,
};

// intro screen background texture Y offsets
float introBackgroundOffsetY[] = {
    160.0, 160.0, 160.0, 160.0, 80.0, 80.0, 80.0, 80.0, 0.0, 0.0, 0.0, 0.0,
};

// table that points to either the "Super Mario 64" or "Game Over" tables
const u8* const* introBackgroundTextureType[] = {mario_title_texture_table, game_over_texture_table};

s8 introBackgroundIndexTable[] = {
    INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO,
    INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO, INTRO_BACKGROUND_SUPER_MARIO,
};

// only one table of indexes listed
s8* introBackgroundTables[] = {introBackgroundIndexTable};

s8 gameOverBackgroundTable[] = {
    INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER,
    INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER, INTRO_BACKGROUND_GAME_OVER,
};

// order of tiles that are flipped from "Game Over" to "Super Mario 64"
s8 gameOverBackgroundFlipOrder[] = {0x00, 0x01, 0x02, 0x03, 0x07, 0x0B, 0x0a, 0x09, 0x08, 0x04, 0x05, 0x06};

Gfx* geo18_title_screen(u32 sp50, struct GraphNode* sp54, UNUSED void* context)
{
	struct GraphNode* graphNode; // sp4c
	Gfx* displayList;	     // sp48
	Gfx* displayListIter;	     // sp44
	Mtx* scaleMat;		     // sp40
	f32* scaleTable1;	     // sp3c
	f32* scaleTable2;	     // sp38
	f32 scaleX;		     // sp34
	f32 scaleY;		     // sp30
	f32 scaleZ;		     // sp2c
	graphNode	= sp54;
	displayList	= NULL;
	displayListIter = NULL;
	scaleTable1	= (f32*)segmented_to_virtual(intro_seg7_table_0700C790);
	scaleTable2	= (f32*)segmented_to_virtual(intro_seg7_table_0700C880);
	if(sp50 != 1)
	{
		gTitleZoomCounter = 0;
	}
	else if(sp50 == 1)
	{
		graphNode->flags = (graphNode->flags & 0xFF) | 0x100;
		scaleMat	 = (Mtx*)AllocDynamic(sizeof(*scaleMat));
		displayList	 = (Gfx*)AllocDynamic(4 * sizeof(*displayList));
		displayListIter	 = displayList;
		if(gTitleZoomCounter >= 0 && gTitleZoomCounter < INTRO_STEPS_ZOOM_IN)
		{
			scaleX = scaleTable1[(gTitleZoomCounter / FRAME_RATE_SCALER_INV) * 3];
			scaleY = scaleTable1[(gTitleZoomCounter / FRAME_RATE_SCALER_INV) * 3 + 1];
			scaleZ = scaleTable1[(gTitleZoomCounter / FRAME_RATE_SCALER_INV) * 3 + 2];
		}
		else if(gTitleZoomCounter >= INTRO_STEPS_ZOOM_IN && gTitleZoomCounter < INTRO_STEPS_HOLD_1)
		{
			scaleX = 1.0f;
			scaleY = 1.0f;
			scaleZ = 1.0f;
		}
		else if(gTitleZoomCounter >= INTRO_STEPS_HOLD_1 && gTitleZoomCounter < INTRO_STEPS_ZOOM_OUT)
		{
			scaleX = scaleTable2[(gTitleZoomCounter - INTRO_STEPS_HOLD_1) / FRAME_RATE_SCALER_INV * 3];
			scaleY = scaleTable2[(gTitleZoomCounter - INTRO_STEPS_HOLD_1) / FRAME_RATE_SCALER_INV * 3 + 1];
			scaleZ = scaleTable2[(gTitleZoomCounter - INTRO_STEPS_HOLD_1) / FRAME_RATE_SCALER_INV * 3 + 2];
		}
		else
		{
			scaleX = 0.0f;
			scaleY = 0.0f;
			scaleZ = 0.0f;
		}
		guScale(scaleMat, scaleX, scaleY, scaleZ);
		gSPMatrix(displayListIter++, scaleMat, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
		gSPDisplayList(displayListIter++, &intro_seg7_dl_0700B3A0);
		gSPPopMatrix(displayListIter++, G_MTX_MODELVIEW);
		gSPEndDisplayList(displayListIter);
		gTitleZoomCounter++;
	}
	return displayList;
}

Gfx* geo18_fade_transition(u32 sp40, struct GraphNode* sp44, UNUSED void* context)
{
	struct GraphNode* graphNode = sp44; // sp3c
	Gfx* displayList	    = NULL; // sp38
	Gfx* displayListIter	    = NULL; // sp34
	if(sp40 != 1)
	{
		gTitleFadeCounter = 0; // D_801B985C
	}
	else if(sp40 == 1)
	{
		displayList	= (Gfx*)AllocDynamic(5 * sizeof(*displayList));
		displayListIter = displayList;
		gSPDisplayList(displayListIter++, wipe_gfx_init);
		gDPSetEnvColor(displayListIter++, 255, 255, 255, gTitleFadeCounter / FRAME_RATE_SCALER_INV);

		if(gTitleFadeCounter == 255 * FRAME_RATE_SCALER_INV)
		{
			graphNode->flags = (graphNode->flags & 0xFF) | 0x100;
			gDPSetRenderMode(displayListIter++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
		}
		else
		{
			graphNode->flags = (graphNode->flags & 0xFF) | 0x500;
			gDPSetRenderMode(displayListIter++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
		}

		gSPDisplayList(displayListIter++, &intro_seg7_dl_0700C6A0);
		gSPEndDisplayList(displayListIter);

		if(gTitleZoomCounter >= 0x13 * FRAME_RATE_SCALER_INV)
		{
			gTitleFadeCounter += 0x1a / FRAME_RATE_SCALER_INV;
			if(gTitleFadeCounter >= 0x100 * FRAME_RATE_SCALER_INV)
			{
				gTitleFadeCounter = 0xFF * FRAME_RATE_SCALER_INV;
			}
		}
	}
	return displayList;
}

Gfx* intro_backdrop_one_image(u32 index, s8* backgroundTable)
{
	Mtx* mtx;			// sp5c
	Gfx* displayList;		// sp58
	Gfx* displayListIter;		// sp54
	const u8* const* vIntroBgTable; // sp50
	s32 i;				// sp4c
	f32 aspect	= GFX_DIMENSIONS_ASPECT_RATIO;
	int num_tiles_h = (((aspect * SCREEN_HEIGHT) + 79) / 80);
	float x_ofs	= (SCREEN_WIDTH / 2) - (aspect * SCREEN_HEIGHT / 2);

	mtx		= (Mtx*)AllocDynamic(sizeof(*mtx));
	displayList	= (Gfx*)AllocDynamic(36 * sizeof(*displayList));
	displayListIter = displayList;
	vIntroBgTable	= (const u8* const*)segmented_to_virtual(introBackgroundTextureType[backgroundTable[0]]);
	guTranslate(mtx, ((index % num_tiles_h) * 80) + x_ofs, (index / num_tiles_h) * 80, 0.0f);
	gSPMatrix(displayListIter++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);
	gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000118);
	for(i = 0; i < 4; ++i)
	{
		gDPLoadTextureBlock(displayListIter++, vIntroBgTable[i], G_IM_FMT_RGBA, G_IM_SIZ_16b, 80, 20, 0, G_TX_CLAMP, G_TX_CLAMP, 7, 6, G_TX_NOLOD, G_TX_NOLOD) gSPDisplayList(displayListIter++, introBackgroundDlRows[i]);
	}
	gSPPopMatrix(displayListIter++, G_MTX_MODELVIEW);
	gSPEndDisplayList(displayListIter);
	return displayList;
}

Gfx* geo18_intro_backdrop(u32 sp48, struct GraphNode* sp4c, UNUSED void* context)
{
	struct GraphNodeMore* graphNode; // sp44
	s32 index;			 // sp40
	s8* backgroundTable;		 // sp3c
	Gfx* displayList;		 // sp38
	Gfx* displayListIter;		 // sp34
	s32 i;				 // sp30
	graphNode	= (struct GraphNodeMore*)sp4c;
	index		= graphNode->unk18 & 0xff; // TODO: word at offset 0x18 of struct GraphNode
	backgroundTable = introBackgroundTables[index];
	displayList	= NULL;
	displayListIter = NULL;
	f32 aspect	= GFX_DIMENSIONS_ASPECT_RATIO;
	int num_tiles_h = (((aspect * SCREEN_HEIGHT) + 79) / 80);

	if(sp48 == 1)
	{
		displayList	      = (Gfx*)AllocDynamic(((num_tiles_h * 3) + 4) * sizeof(*displayList));
		displayListIter	      = displayList;
		graphNode->node.flags = (graphNode->node.flags & 0xFF) | 0x100;
		gSPDisplayList(displayListIter++, &wipe_gfx_init);
		gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000100);
		for(i = 0; i < num_tiles_h * 3; ++i)
		{
			gSPDisplayList(displayListIter++, intro_backdrop_one_image(i, backgroundTable));
		}
		gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000190);
		gSPEndDisplayList(displayListIter);
	}
	return displayList;
}

Gfx* geo18_game_over_tile(u32 sp40, struct GraphNode* sp44, UNUSED void* context)
{
	struct GraphNode* graphNode; // sp3c
	Gfx* displayList;	     // sp38
	Gfx* displayListIter;	     // sp34
	s32 j;			     // sp30
	s32 i;			     // sp2c
	graphNode	= sp44;
	displayList	= NULL;
	displayListIter = NULL;
	f32 aspect	= GFX_DIMENSIONS_ASPECT_RATIO;
	int num_tiles_h = (((aspect * SCREEN_HEIGHT) + 79) / 80);

	if(sp40 != 1)
	{
		gGameOverFrameCounter = 0;
		gGameOverTableIndex   = -2;
		for(i = 0; i < (s32)sizeof(gameOverBackgroundTable); ++i)
		{
			gameOverBackgroundTable[i] = INTRO_BACKGROUND_GAME_OVER;
		}
	}
	else
	{
		displayList	= (Gfx*)AllocDynamic(((num_tiles_h * 3) + 4) * sizeof(*displayList));
		displayListIter = displayList;
		if(gGameOverTableIndex == -2)
		{
			if(gGameOverFrameCounter == 180 * FRAME_RATE_SCALER_INV)
			{
				gGameOverTableIndex++;
				gGameOverFrameCounter = 0;
			}
		}
		else
		{
			// transition tile from "Game Over" to "Super Mario 64"
			if(gGameOverTableIndex != 11 && !(gGameOverFrameCounter % (2 * FRAME_RATE_SCALER_INV)))
			{
				gGameOverTableIndex++;
				gameOverBackgroundTable[gameOverBackgroundFlipOrder[gGameOverTableIndex]] = INTRO_BACKGROUND_SUPER_MARIO;
			}
		}
		if(gGameOverTableIndex != 11)
		{
			gGameOverFrameCounter++;
		}
		graphNode->flags = (graphNode->flags & 0xFF) | 0x100;
		gSPDisplayList(displayListIter++, &wipe_gfx_init);
		gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000100);
		for(j = 0; j < (s32)num_tiles_h * 3; ++j)
		{
			gSPDisplayList(displayListIter++, intro_backdrop_one_image(j, gameOverBackgroundTable));
		}
		gSPDisplayList(displayListIter++, &title_screen_bg_dl_0A000190);
		gSPEndDisplayList(displayListIter);
	}
	return displayList;
}

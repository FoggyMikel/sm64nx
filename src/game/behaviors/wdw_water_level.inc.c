// wdw_water_level.c.inc
#include "game/motor.h"

// called when WDW is loaded.
void bhv_init_changing_water_level_loop(void)
{
	if(gCurrentObject->oAction == 0)
	{
		if(gEnvironmentRegions != NULL)
			gCurrentObject->oAction++;
	}
	else if(gCurrentObject->oTimer < 10 * FRAME_RATE_SCALER_INV)
	{
		*gEnvironmentLevels = gEnvironmentRegions[6];
	}
	else
	{
		gEnvironmentRegions[6] = *gEnvironmentLevels + sins(o->oWaterLevelTriggerUnkF4) * 20.0f;
		gCurrentObject->oWaterLevelTriggerUnkF4 += 0x200 / FRAME_RATE_SCALER_INV;
	}
}

void bhv_water_level_diamond_loop(void)
{
	if(gEnvironmentRegions != NULL)
	{
		switch(o->oAction)
		{
			case WATER_LEVEL_DIAMOND_ACT_INIT:
				o->oFaceAngleYaw		      = 0;
				o->oWaterLevelTriggerTargetWaterLevel = (s32)o->oPosY;
				if(o->oTimer > 10 * FRAME_RATE_SCALER_INV)
					o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_IDLE
				break;
			case WATER_LEVEL_DIAMOND_ACT_IDLE:
				if(s_hitcheck(o, gMarioObject))
				{
					if(gWDWWaterLevelChanging == 0)
					{
						o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL
						gWDWWaterLevelChanging = 1;
					}
				}
				break;
			case WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL:
				o->oAngleVelYaw	    = 0;
				*gEnvironmentLevels = (s32)approach_f32_symmetric((f32)*gEnvironmentLevels, (f32)o->oWaterLevelTriggerTargetWaterLevel, 10.0f * FRAME_RATE_SCALER);
				if(*gEnvironmentLevels == o->oWaterLevelTriggerTargetWaterLevel)
				{
					if((s16)o->oFaceAngleYaw == 0)
						o->oAction++; // Sets to WATER_LEVEL_DIAMOND_ACT_IDLE_SPINNING
					else
						o->oAngleVelYaw = 0x800;
				}
				else
				{
					if(o->oTimer == 0)
						objsound(SOUND_GENERAL_WATER_LEVEL_TRIG);
					else
					{
						if(*gEnvironmentLevels > o->oWaterLevelTriggerTargetWaterLevel)
							objsound_level(SOUND_ENV_WATER_DRAIN);
						else
							objsound_level(SOUND_ENV_WATER_DRAIN); // same as above
					}
					o->oAngleVelYaw = 0x800;
					SendMotorVib(2);
				}
				break;
			case WATER_LEVEL_DIAMOND_ACT_IDLE_SPINNING:
				if(!s_hitcheck(o, gMarioObject))
				{
					gWDWWaterLevelChanging = 0;
					o->oAction	       = WATER_LEVEL_DIAMOND_ACT_IDLE;
					o->oAngleVelYaw	       = 0;
				}
				break;
		}
		o->oFaceAngleYaw += o->oAngleVelYaw / FRAME_RATE_SCALER_INV;
	}
}

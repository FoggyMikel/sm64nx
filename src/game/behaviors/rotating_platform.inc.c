// rotating_platform.c.inc

#include "levels/wf/header.h"
#include "levels/wdw/header.h"

struct WFRotatingPlatformData sWFRotatingPlatformData[] = {{0, 100, wf_seg7_collision_rotating_platform, 2000}, {0, 150, wdw_seg7_collision_070186B4, 1000}};

void bhv_wf_rotating_wooden_platform_loop(void)
{
	s32 timer = o->oTimer;

	if(o->oAction == 0)
	{
		o->oAngleVelYaw = 0;
		if(timer > 60 * FRAME_RATE_SCALER_INV)
		{
			o->oAction++;
		}
	}
	else
	{
		s32 a		= o->oFaceAngleYaw;
		o->oAngleVelYaw = 0x100;
		if(timer > 127 * FRAME_RATE_SCALER_INV)
		{
			o->oAction = 0;
		}

		objsound_level(SOUND_ENV_ELEVATOR2);
	}
	obj_rotate_face_angle_using_vel();
}

void bhv_rotating_platform_loop(void)
{
	s8 sp1F = o->oBehParams >> 24;

	if(o->oTimer == 0)
	{
		set_object_collision_data(o, sWFRotatingPlatformData[o->oBehParams2ndByte].collisionData);
		o->oCollisionDistance = sWFRotatingPlatformData[o->oBehParams2ndByte].collisionDistance;
		s_set_scale(sWFRotatingPlatformData[o->oBehParams2ndByte].scale * 0.01f);
	}

	o->oAngleVelYaw = sp1F << 4;
	o->oFaceAngleYaw += o->oAngleVelYaw * FRAME_RATE_SCALER;
}

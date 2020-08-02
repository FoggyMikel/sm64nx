#pragma once
#include "types.h"

struct PlayerRecord
{
	PlayerRecord();
	~PlayerRecord();

	void initPlayer();
	void initPlayerWorks();

	s32 isLast1AnimeFrame();
	s32 isLast2AnimeFrame();
	s16 setAnimation(s32 targetAnimID);
	s16 setAnimation(s32 targetAnimID, s32 accel);
	void anim_reset();
	void set_anim_to_frame(s16 animFrame);
	s32 checkAnimeFrame(s16 animFrame);
	void update_mario_pos_for_anim();
	s16 return_mario_anim_y_translation();
	void startSoundEffect(u32 soundBits, u32 flags);
	void startJumpVoice();
	void setSpeedVolume();
	void specialEffect(u32 soundBits, u32 waveParticleType);
	void trigSpecialEffect(u32 soundBits, u32 waveParticleType);
	void startLandingEffect(u32 soundBits);
	void trigStartLandingEffect(u32 soundBits);
	void startDowningEffect(u32 soundBits);
	void trigStartDowningEffect(u32 soundBits);
	void startJumpingEffect(s32 primarySoundBits, s32 scondarySoundBits);
	void setPlayerVelocity(f32 speed);
	s32 getSlipCode();
	u32 checkGroundSurface();

	s32 isFrontSlip(s32 turnYaw);
	u32 isSlipStart();
	s32 isSlipLimit();
	s32 isJumpMiss();
	f32 checkPlayerAround(s16 angleFromMario, f32 distFromMario);
	s16 groundGradient(s16 yawOffset);
	void resetSpecialCameraMode();
	void set_steep_jump_action();
	s32 changePlayerTriJump();
	s32 changePlayerJumping(u32 action, u32 actionArg);
	s32 changePlayerDropping(u32 action, u32 actionArg);
	s32 changePlayerDamage(u32 action, u32 actionArg, s16 hurtCounter);
	s32 checkAllMotions();
	s32 checkAllHoldMotions();
	s32 changeFieldMode();
	s32 changeWaterMode();

	/*0x00*/ u16 number;
	/*0x02*/ u16 input;
	/*0x04*/ u32 flags;
	/*0x08*/ u32 particleFlags;
	/*0x0C*/ u32 status;
	/*0x10*/ u32 oldstatus;
	/*0x14*/ u32 terrainSoundAddend;
	/*0x18*/ u16 actionState;
	/*0x1A*/ u16 actionTimer;
	/*0x1C*/ u32 actionArg;
	/*0x20*/ f32 intendedMag;
	/*0x24*/ s16 intendedYaw;
	/*0x26*/ s16 invincTimer;
	/*0x28*/ u8 framesSinceA;
	/*0x29*/ u8 framesSinceB;
	/*0x2A*/ u8 wallKickTimer;
	/*0x2B*/ u8 doubleJumpTimer;
	/*0x2C*/ Vec3s faceAngle;
	/*0x32*/ Vec3s angleVel;
	/*0x38*/ s16 slideYaw;
	/*0x3A*/ s16 twirlYaw;
	/*0x3C*/ Vec3f pos;
	/*0x48*/ Vec3f vel;
	/*0x54*/ f32 forwardVel;
	/*0x58*/ f32 slideVelX;
	/*0x5C*/ f32 slideVelZ;
	/*0x60*/ struct Surface* wall;
	/*0x64*/ struct Surface* ceil;
	/*0x68*/ struct Surface* floor;
	/*0x6C*/ f32 ceilHeight;
	/*0x70*/ f32 floorHeight;
	/*0x74*/ s16 floorAngle;
	/*0x76*/ s16 waterLevel;
	/*0x78*/ struct Object* interactObj;
	/*0x7C*/ struct Object* heldObj;
	/*0x80*/ struct Object* usedObj;
	/*0x84*/ struct Object* riddenObj;
	/*0x88*/ struct Object* marioObj;
	/*0x8C*/ struct SpawnInfo* spawnInfo;
	/*0x90*/ struct Area* area;
	/*0x94*/ struct PlayerCameraState* statusForCamera;
	/*0x98*/ struct MarioBodyState* marioBodyState;
	/*0x9C*/ struct sm64::hid::Controller* controller;
	/*0xA0*/ struct MarioAnimation* animation;
	/*0xA4*/ u32 collidedObjInteractTypes;
	/*0xA8*/ s16 numCoins;
	/*0xAA*/ s16 numStars;
	/*0xAC*/ s8 numKeys; // Unused key mechanic
	/*0xAD*/ s8 numLives;
	/*0xAE*/ s16 health;
	/*0xB0*/ s16 unkB0;
	/*0xB2*/ u8 hurtCounter;
	/*0xB3*/ u8 healCounter;
	/*0xB4*/ u16 squishTimer;
	/*0xB5*/ u8 fadeWarpOpacity;
	/*0xB6*/ u16 capTimer;
	/*0xB8*/ s16 reserved2;
	/*0xBC*/ f32 fallpos;
	/*0xC0*/ f32 sinking;
	/*0xC4*/ f32 gravity;

	// actions moving
	s16 func_80263A50();
	void playerRunningSound(s16 frame1, s16 frame2);
	void func_80263C14();
	s32 begin_walking_action(f32 forwardVel, u32 action, u32 actionArg);
	void check_ledge_climb_down();
	void slide_bonk(u32 fastAction, u32 slowAction);
	s32 set_triple_jump_action(UNUSED u32 action, UNUSED u32 actionArg);
	s32 set_special_triple_jump_action(UNUSED u32 action, UNUSED u32 actionArg);
	void update_sliding_angle(f32 accel, f32 lossFactor);
	s32 update_sliding(f32 stopSpeed);
	void apply_slope_accel();
	s32 apply_landing_accel(f32 frictionFactor);
	void update_shell_speed();
	s32 apply_slope_decel(f32 decelCoef);
	s32 update_decelerating_speed();
	void update_walking_speed();
	s32 should_begin_sliding();
	s32 analog_stick_held_back();
	s32 check_ground_dive_or_punch();
	s32 begin_braking_action();
	void func_802652F0();
	void func_8026570C();
	void func_80265980();
	void func_802659E8(Vec3f startPos);
	void func_80265C28(s16 startYaw);
	void func_80265DBC(s16 startYaw);
	s32 act_walking();
	s32 act_move_punching();
	s32 act_hold_walking();
	s32 act_hold_heavy_walking();
	s32 act_turning_around();
	void func_80267814();
	void common_slide_action(u32 endAction, u32 airAction, s32 animation);
	s32 common_slide_action_with_jump(u32 stopAction, u32 jumpAction, u32 airAction, s32 animation);

	// actions_object
	void animated_stationary_ground_step(s32 animation, u32 endAction);
	s32 mario_update_punch_sequence();
	s32 act_punching();
	s32 act_picking_up();
	s32 act_dive_picking_up();
	s32 act_placing_down();
	s32 act_throwing();
	s32 act_heavy_throw();
	s32 act_stomach_slide_stop();
	s32 act_picking_up_bowser();
	s32 act_holding_bowser();
	s32 act_releasing_bowser();
	s32 check_common_object_cancels();
	s32 playerActnMain();

	// actions_stationary
	s32 func_802604E0();
	s32 func_802606DC();
	s32 act_idle();
	void func_80260BC4(u32 actionState, s32 animFrame, u32 sound);
	s32 act_start_sleeping();
	s32 act_sleeping();
	s32 act_waking_up();
	s32 act_shivering();
	s32 act_coughing();
	s32 act_hold_idle();
	s32 act_hold_heavy_idle();
	s32 act_standing_against_wall();
	s32 act_in_quicksand();
	s32 act_crouching();
	s32 act_panting();
	s32 act_hold_panting_unused();
	void func_80261D70(s32 animID, u32 action);
	s32 act_braking_stop();
	s32 act_butt_slide_stop();
	s32 func_80261F8C();
	s32 act_slide_kick_slide_stop();
	s32 act_start_crouching();
	s32 act_stop_crouching();
	s32 act_start_crawling();
	s32 act_stop_crawling();
	s32 act_shockwave_bounce();
	s32 func_802627B8(s32 arg1, u32 action);
	s32 func_80262828(u32 action);
	s32 act_jump_land_stop();
	s32 act_double_jump_land_stop();
	s32 act_side_flip_land_stop();
	s32 act_freefall_land_stop();
	s32 act_triple_jump_land_stop();
	s32 act_backflip_land_stop();
	s32 act_lava_boost_land();
	s32 act_long_jump_land_stop();
	s32 func_80262CEC();
	s32 func_80262DE4();
	s32 act_air_throw_land();
	s32 act_twirl_land();
	s32 act_ground_pound_land();
	s32 act_first_person();
	s32 func_80263378();
	s32 playerWaitMain();

	// actions_submerged
	void set_swimming_at_surface_particles(u32 particleFlag);
	s32 swimming_near_surface();
	f32 get_buoyancy();
	u32 perform_water_full_step(Vec3f nextPos);
	void apply_water_current(Vec3f velocity);
	u32 perform_water_step();
	void func_80270504();
	void stationary_slow_down();
	void update_swimming_speed(f32 decelThreshold);
	void update_swimming_yaw();
	void update_swimming_pitch();
	void common_idle_step(s32 animation, s32 arg);
	s32 act_water_idle();
	s32 act_hold_water_idle();
	s32 act_water_action_end();
	s32 act_hold_water_action_end();
	void func_8027107C();
	void func_802710CC();
	void common_swimming_step(s16 swimStrength);
	void func_802713A8();
	s32 check_water_jump();
	s32 act_breaststroke();
	s32 act_swimming_end();
	s32 act_flutter_kick();
	s32 act_hold_breaststroke();
	s32 act_hold_swimming_end();
	s32 act_hold_flutter_kick();
	s32 act_water_shell_swimming();
	s32 check_water_grab();
	s32 act_water_throw();
	s32 act_water_punch();
	void common_water_knockback_step(s32 animation, u32 endAction, s32 arg3);
	s32 act_backward_water_kb();
	s32 act_forward_water_kb();
	s32 act_water_shocked();
	s32 act_drowning();
	s32 act_water_death();
	s32 act_water_plunge();
	s32 act_caught_in_whirlpool();
	void play_metal_water_jumping_sound(u32 landing);
	void play_metal_water_walking_sound();
	void update_metal_water_walking_speed();
	s32 update_metal_water_jump_speed();
	s32 act_metal_water_standing();
	s32 act_hold_metal_water_standing();
	s32 act_metal_water_walking();
	s32 act_hold_metal_water_walking();
	s32 act_metal_water_jump();
	s32 act_hold_metal_water_jump();
	s32 act_metal_water_falling();
	s32 act_hold_metal_water_falling();
	s32 act_metal_water_jump_land();
	s32 act_hold_metal_water_jump_land();
	s32 act_metal_water_fall_land();
	s32 act_hold_metal_water_fall_land();
	s32 check_common_submerged_cancels();
	s32 PL_PlayerSwimMain();

	// action_airborne
	void play_flip_sounds(s16 frame1, s16 frame2, s16 frame3);
	void play_far_fall_sound();
	void func_u_8026A090();
	s32 lava_boost_on_wall();
	s32 check_fall_damage(u32 hardFallAction);
	s32 check_kick_or_dive_in_air();
	s32 should_get_stuck_in_ground();
	s32 check_fall_damage_or_get_stuck(u32 hardFallAction);
	s32 check_horizontal_wind();
	void update_air_with_turn();
	void update_air_without_turn();
	void update_lava_boost_or_twirling();
	void update_flying_yaw();
	void update_flying_pitch();
	void update_flying();
	u32 common_air_action_step(u32 landAction, s32 animation, u32 stepArg);
	s32 act_jump();
	s32 act_double_jump();
	s32 act_triple_jump();
	s32 act_backflip();
	s32 act_freefall();
	s32 act_hold_jump();
	s32 act_hold_freefall();
	s32 act_side_flip();
	s32 act_wall_kick_air();
	s32 act_long_jump();
	s32 act_riding_shell_air();
	s32 act_twirling();
	s32 act_dive();
	s32 act_air_throw();
	s32 act_water_jump();
	s32 act_hold_water_jump();
	s32 act_steep_jump();
	s32 act_ground_pound();
	s32 act_burning_jump();
	s32 act_burning_fall();
	s32 act_crazy_box_bounce();
	u32 common_air_knockback_step(u32 landAction, u32 hardFallAction, s32 animation, f32 speed);
	s32 func_8026CDFC();
	s32 act_backward_air_kb();
	s32 act_forward_air_kb();
	s32 act_hard_backward_air_kb();
	s32 act_hard_forward_air_kb();
	s32 act_thrown_backward();
	s32 act_thrown_forward();
	s32 act_soft_bonk();
	s32 act_getting_blown();
	s32 act_air_hit_wall();
	s32 act_forward_rollout();
	s32 act_backward_rollout();
	s32 act_butt_slide_air();
	s32 act_hold_butt_slide_air();
	s32 act_lava_boost();
	s32 act_slide_kick();
	s32 act_jump_kick();
	s32 act_shot_from_cannon();
	s32 act_flying();
	s32 act_riding_hoot();
	s32 act_flying_triple_jump();
	s32 act_top_of_pole_jump();
	s32 act_vertical_wind();
	s32 act_special_triple_jump();
	s32 check_common_airborne_cancels();
	s32 playerJumpMain();

	// actions automatic
	void add_tree_leaf_particles();
	void play_climbing_sounds(s32 b);
	s32 set_pole_position(f32 offsetY);
	s32 act_holding_pole();
	s32 act_climbing_pole();
	s32 act_grab_pole_slow();
	s32 act_grab_pole_fast();
	s32 act_top_of_pole_transition();
	s32 act_top_of_pole();
	s32 perform_hanging_step(Vec3f nextPos);
	s32 update_hang_moving();
	void update_hang_stationary();
	s32 act_start_hanging();
	s32 act_hanging();
	s32 act_hang_moving();
	s32 let_go_of_ledge();
	void func_8025F0DC();
	void func_8025F188();
	void update_ledge_climb(s32 animation, u32 endAction);
	s32 act_ledge_grab();
	s32 act_ledge_climb_slow();
	s32 act_ledge_climb_down();
	s32 act_ledge_climb_fast();
	s32 act_grabbed();
	s32 act_in_cannon();
	s32 act_tornado_twirling();
	s32 check_common_automatic_cancels();
	s32 playerSpecMain();

	// actions cutscene
	s32 get_star_collection_dialog();
	void handle_save_menu();
	struct Object* spawn_obj_at_mario_rel_yaw(s32 model, const BehaviorScript* behavior, s16 relYaw);
	void cutscene_take_cap_off();
	void cutscene_put_cap_on();
	s32 act_reading_npc_dialog();
	s32 act_waiting_for_dialog();
	s32 act_disappeared();
	s32 act_reading_automatic_dialog();
	s32 act_reading_sign();
	s32 act_debug_free_move();
	void general_star_dance_handler(s32 isInWater);
	s32 act_star_dance();
	s32 act_star_dance_water();
	s32 act_fall_after_star_grab();
	s32 common_death_handler(s32 animation, s32 frameToDeathWarp);
	s32 act_standing_death();
	s32 act_electrocution();
	s32 act_suffocation();
	s32 act_death_on_back();
	s32 act_death_on_stomach();
	s32 act_quicksand_death();
	s32 act_eaten_by_bubba();
	s32 launch_mario_until_land(s32 endAction, s32 animation, f32 forwardVel);
	s32 act_unlocking_key_door();
	s32 act_unlocking_star_door();
	s32 act_entering_star_door();
	s32 act_going_through_door();
	s32 act_warp_door_spawn();
	s32 act_emerge_from_pipe();
	s32 act_spawn_spin_airborne();
	s32 act_spawn_spin_landing();
	s32 act_exit_airborne();
	s32 act_falling_exit_airborne();
	s32 act_exit_land_save_dialog();
	s32 act_death_exit();
	s32 act_unused_death_exit();
	s32 act_falling_death_exit();
	s32 act_special_exit_airborne();
	s32 act_special_death_exit();
	s32 act_spawn_no_spin_airborne();
	s32 act_spawn_no_spin_landing();
	s32 act_bbh_enter_spin();
	s32 act_bbh_enter_jump();
	s32 act_teleport_fade_out();
	s32 act_teleport_fade_in();
	s32 act_shocked();
	s32 act_squished();
	s32 act_putting_on_cap();
	void stuck_in_ground_handler(s32 animation, s32 unstuckFrame, s32 target2, s32 target3, s32 endAction);
	s32 act_head_stuck_in_ground();
	s32 act_butt_stuck_in_ground();
	s32 act_feet_stuck_in_ground();
	void advance_cutscene_step();
	void intro_cutscene_hide_hud_and_mario();
	void intro_cutscene_peach_lakitu_scene();
	void intro_cutscene_raise_pipe();
	void intro_cutscene_jump_out_of_pipe();
	void intro_cutscene_land_outside_pipe();
	void intro_cutscene_lower_pipe();
	void intro_cutscene_set_mario_to_idle();
	s32 act_intro_cutscene();
	void jumbo_star_cutscene_falling();
	s32 jumbo_star_cutscene_taking_off();
	s32 jumbo_star_cutscene_flying();
	s32 act_jumbo_star_cutscene();
	void end_peach_cutscene_mario_falling();
	void end_peach_cutscene_mario_landing();
	void end_peach_cutscene_summon_jumbo_star();
	void end_peach_cutscene_spawn_peach();
	void end_peach_cutscene_descend_peach();
	void end_peach_cutscene_run_to_peach();
	void end_peach_cutscene_dialog_1();
	void end_peach_cutscene_dialog_2();
	void end_peach_cutscene_kiss_from_peach();
	void end_peach_cutscene_star_dance();
	void end_peach_cutscene_dialog_3();
	void end_peach_cutscene_run_to_castle();
	void end_peach_cutscene_fade_out();
	s32 act_end_peach_cutscene();
	s32 act_credits_cutscene();
	s32 act_end_waving_cutscene();
	s32 check_for_instant_quicksand();
	s32 playerDemoMain();

	// mario_step
	void playerRefrection(u32 negateSpeed);
	u32 checkPlayerSinking(f32 sinkingSpeed);
	u32 fallFromHeavySlope(u32 action, u32 actionArg);
	u32 quicksandProcess();
	u32 gustProcess();
	void stopProcess();
	s32 waitProcess();
	s32 perform_ground_quarter_step(Vec3f nextPos);
	s32 walkProcess();
	u32 check_ledge_grab(struct Surface* wall, Vec3f intendedPos, Vec3f nextPos);
	s32 perform_air_quarter_step(Vec3f intendedPos, u32 stepArg);
	void apply_twirl_gravity();
	u32 should_strengthen_gravity_for_jump_ascent();
	void apply_gravity();
	void apply_vertical_wind();
	s32 jumpProcess(u32 stepArg);
	void set3DSpeed();
	void set2DSpeed();

	//	protected:
	void update_mario_info_for_cam();
	void mario_reset_bodystate();
	void sink_mario_in_quicksand();
	void squish_mario_model();
	void set_mario_y_vel_based_on_fspeed(f32 initialVelY, f32 multiplier);

	u32 set_mario_action_airborne(u32 action, u32 actionArg);
	u32 set_mario_action_moving(u32 action, u32 actionArg);
	u32 set_mario_action_submerged(u32 action, u32 actionArg);
	u32 set_mario_action_cutscene(u32 action, u32 actionArg);
	u32 ChangePlayerStatus(u32 action, u32 actionArg);
	s32 act_finish_turning_around();
	s32 act_braking();
	s32 act_decelerating();
	s32 act_hold_decelerating();
	s32 act_riding_shell_ground();
	s32 act_crawling();
	s32 act_burning_ground();
	s32 act_butt_slide();
	s32 act_hold_butt_slide();
	s32 act_crouch_slide();
	s32 act_slide_kick_slide();
	s32 stomach_slide_action(u32 stopAction, u32 airAction, s32 animation);
	s32 act_stomach_slide();
	s32 act_hold_stomach_slide();
	s32 act_dive_slide();
	s32 common_ground_knockback_action(s32 animation, s32 arg2, s32 arg3, s32 arg4);
	s32 act_hard_backward_ground_kb();
	s32 act_hard_forward_ground_kb();
	s32 act_backward_ground_kb();
	s32 act_forward_ground_kb();
	s32 act_soft_backward_ground_kb();
	s32 act_soft_forward_ground_kb();
	s32 act_ground_bonk();
	s32 act_death_exit_land();

	u32 common_landing_action(s16 animation, u32 airAction);
	s32 common_landing_cancels(struct LandingAction* landingAction, s32 (PlayerRecord::*setAPressAction)(u32, u32));

	s32 act_jump_land();
	s32 act_freefall_land();
	s32 act_side_flip_land();
	s32 act_hold_jump_land();
	s32 act_hold_freefall_land();
	s32 act_long_jump_land();
	s32 act_double_jump_land();
	s32 act_triple_jump_land();
	s32 act_backflip_land();
	s32 quicksand_jump_land_action(s32 animation1, s32 animation2, u32 endAction, u32 airAction);
	s32 act_quicksand_jump_land();
	s32 act_hold_quicksand_jump_land();
	s32 check_common_moving_cancels();
	s32 playerMoveMain();

	void update_mario_button_inputs();
	void update_mario_joystick_inputs();
	void update_mario_geometry_inputs();
	void update_mario_inputs();

	void set_submerged_cam_preset_and_spawn_bubbles();
	void update_mario_health();

	u32 update_and_return_cap_flags();
	void mario_update_hitbox_and_cap_model();
};

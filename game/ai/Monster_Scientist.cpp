
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

class rvMonsterScientist : public idAI {
public:

	CLASS_PROTOTYPE( rvMonsterScientist );

	rvMonsterScientist ( void );
	
	void				Spawn				( void );
	
	virtual void		OnDeath				( void );
	
	// Add some dynamic externals for debugging
	virtual void		GetDebugInfo		( debugInfoProc_t proc, void* userData );

private:

	CLASS_STATES_PROTOTYPE ( rvMonsterScientist );
};

CLASS_DECLARATION( idAI, rvMonsterScientist )
END_CLASS

/*
================
rvMonsterScientist::rvMonsterScientist
================
*/
rvMonsterScientist::rvMonsterScientist ( void ) {
}

/*
================
rvMonsterScientist::Spawn
================
*/
void rvMonsterScientist::Spawn ( void ) {
	PlayEffect ( "fx_fly", animator.GetJointHandle ( "effects_bone" ), true );
} 

/*
================
rvMonsterScientist::OnDeath
================
*/
void rvMonsterScientist::OnDeath ( void ) {
	StopEffect ( "fx_fly" );
	

		const char *value;
		idEntity* dropitem = NULL;
		idDict	  args;


		int itemDropped = rand() % 14;
		switch (itemDropped){
		case 0:
			value = "weapon_blaster";
			break;
		case 1:
			value = "weapon_dmg";
			break;
		case 2:
			value = "weapon_grenadelauncher";
			break;
		case 3:
			value = "weapon_hyperblaster";
			break;
		case 4:
			value = "weapon_lightninggun";
			break;
		case 5:
			value = "weapon_nailgun";
			break;
		case 6:
			value = "weapon_napalmgun";
			break;
		case 7:
			value = "weapon_railgun";
			break;
		case 8:
			value = "weapon_rocketlauncher";
			break;
		case 9:
			value = "weapon_shotgun";
			break;
		case 10:
			value = "powerup_quad_damage";
			break;
		case 11:
			value = "powerup_haste";
			break;
		case 12:
			value = "powerup_regeneration";
			break;
		case 13:
			value = "powerup_invisibility";
			break;
		case 14:
			value = "powerup_team_damage_mod";
			break;
		}


		args.Set("classname", value);
		args.SetVector("origin", GetPhysics()->GetOrigin());
		args.SetInt("angle", move.current_yaw);
		gameLocal.SpawnEntityDef(args, &dropitem);



	idAI::OnDeath ( );
}

/*
================
rvMonsterScientist::GetDebugInfo
================
*/
void rvMonsterScientist::GetDebugInfo	( debugInfoProc_t proc, void* userData ) {
	// Base class first
	idAI::GetDebugInfo ( proc, userData );
}

/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( rvMonsterScientist )
END_CLASS_STATES

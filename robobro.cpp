//ev8 this is the entity class for my robot helper
#include "cbase.h"

class CRobobro : public CBaseAnimating
{
public:
	DECLARE_CLASS( CRobobro, CBaseAnimating );
	DECLARE_DATADESC();
 
	CRobobro()
	{
		m_bfetching= false;
	}
	//simple boiler plate entity declarations
	void Spawn( void );
	void Precache( void );
	void Activate(void);
	void MoveThink( void );
	void UseFunc( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	// Input function
	void InputToggle( inputdata_t &inputData );
 
private:
 
	bool	m_bfetching;
	float	m_flNextChangeTime;
	float	m_flNextshootTime;
	CBaseEntity *target;
};
 
LINK_ENTITY_TO_CLASS( Robro, CRobobro );
 
// Start of our data description for the class
BEGIN_DATADESC( CRobobro )
 
	// Save/restore our active state
	DEFINE_FIELD( m_bfetching, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextChangeTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextshootTime, FIELD_TIME ),

 
	// Links our input name from Hammer to our input member function
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
 
	// Declare our think function
	DEFINE_THINKFUNC( MoveThink ),
	DEFINE_USEFUNC( UseFunc ),
	
	
END_DATADESC()
 
// Name of our entity's model
#define	ENTITY_MODEL	"models/combine_scanner.mdl"
 
//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
//ev8 loads the model, i'll add sounds later

void CRobobro::Precache( void )
{
	PrecacheModel( ENTITY_MODEL );
 
	BaseClass::Precache();
}
// ev8 boiler plate function for when entity is loaded,
 void CRobobro::Activate()
 {
	 SetThink( &CRobobro::MoveThink );
	 CBaseAnimating::Activate();
 }
//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
// ev8 spawns my entity and gives it a bounding box for world collisions but not physics,
 // apparently getting something to fly and respond to physical touchs is very hard to do
 void CRobobro::Spawn( void )
{
	Precache();
 
	SetModel( ENTITY_MODEL );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, -Vector(20,20,20), Vector(20,20,20) );
	SetSequence( LookupSequence("idle") );
	// Set the animation speed to 100%
	SetPlaybackRate( 1.0f );
	// Tell the client to animate this model
	UseClientSideAnimation();
	SetThink( &CRobobro::MoveThink );
	SetNextThink( gpGlobals->curtime + 0.05f );
	SetUse( &CRobobro::Use );
 
		// Start moving
		SetMoveType( MOVETYPE_FLY );
 


}
 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ev8 this is the use function that would allow the player to interact with robobro but i'm haveing some inheritence issues trying accessing emmeber functions of base player class
 void CRobobro::UseFunc( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{	
	if (pActivator->IsPlayer()){
	CBasePlayer *pPlayer = (CBasePlayer *)this->GetOwnerEntity() ;
		
	CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	//ev8 now i have a reference to the weapon robobro will store, i'd store this localy and then remove the weapon from the player
	//pPlayer->Weapon_DropPrimary();
	// unfortunately this function is broken, as in  it's in the engine but valve itself acknowledges that it dosen't work.
	//i can't really find a work around because of the way items and weapons are handled in source
	}
}
// ev8 this is robobro's think function, it handles  deciding where robobro is going, what he is doing and who he is attacking
void CRobobro::MoveThink( void )
{
	
	float distance = GetAbsOrigin().DistTo( this->GetOwnerEntity()->WorldSpaceCenter());
	CBaseEntity *pEntity = NULL;
	if (distance<300){//checks to make sure robobro is close enough to the player to do helpfulsyuff
	for ( CEntitySphereQuery sphere( GetAbsOrigin(), 50 ); ( pEntity = sphere.GetCurrentEntity() ) != NULL; sphere.NextEntity() )
	{
		if ( pEntity->Classify() == CLASS_NONE )// ev8 added this flag to health and ammor crates but apparent it's also used on all the prop items so i need to find a way of identifying items
		{
			Vector	vTargetDir;
			VectorSubtract( pEntity->GetAbsOrigin(), GetAbsOrigin(), vTargetDir );
	Vector	vDir	= GetAbsVelocity();
	
	Vector	vNewVelocity = vDir;
	
	
			vNewVelocity = vTargetDir;
	

	QAngle	finalAngles;
	VectorAngles( vNewVelocity, finalAngles );
	SetAbsAngles( finalAngles );
		Vector vecForward; 
	AngleVectors( GetLocalAngles(), &vecForward );
	SetAbsVelocity( vecForward * 10 );
			//chase item to get for player
			break;
		}else if ( pEntity->m_takedamage != DAMAGE_NO && pEntity->Classify() != CLASS_PLAYER){//ev8 only way i could find to check for a possible target
			// attack code goes here
		}
		
	}

	}else{// to far away chase player
		Vector	vTargetDir;
	VectorSubtract( this->GetOwnerEntity()->GetAbsOrigin(), GetAbsOrigin(), vTargetDir );
	Vector	vDir	= GetAbsVelocity();
	
	Vector	vNewVelocity = vDir;
	
	
			vNewVelocity = vTargetDir;
	

	QAngle	finalAngles;
	VectorAngles( vNewVelocity, finalAngles );
	SetAbsAngles( finalAngles );
		Vector vecForward; 
	AngleVectors( GetLocalAngles(), &vecForward );
	SetAbsVelocity( vecForward * 200 );
	
	}
	// Think every 20Hz
	SetNextThink( gpGlobals->curtime + 0.05f );
}
 
//-----------------------------------------------------------------------------
// Purpose: Toggle the movement of the entity
//-----------------------------------------------------------------------------
void CRobobro::InputToggle( inputdata_t &inputData )
{
	
}

// ev8 a console command to spawn robobro, i'll move the spaning code to the player spawn code later, so the player and robobro spawn simultaniuly.
CON_COMMAND(create_Robro, "Creates a roBro")
{
	Vector vecForward;
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if(!pPlayer)
	{
		Warning("Could not determine calling player!\n");
		return;
	}
 
	AngleVectors( pPlayer->EyeAngles(), &vecForward );
	CBaseEntity *pEnt = CreateEntityByName( "Robro" );
	if ( pEnt )
	{
		pEnt->SetOwnerEntity(pPlayer);
		Vector vecOrigin = pPlayer->GetAbsOrigin() + vecForward * 256 + Vector(0,0,64);
		QAngle vecAngles(0, pPlayer->GetAbsAngles().y - 90, 0);
		pEnt->SetAbsOrigin(vecOrigin);
		pEnt->SetAbsAngles(vecAngles);
		DispatchSpawn(pEnt);
	}
}


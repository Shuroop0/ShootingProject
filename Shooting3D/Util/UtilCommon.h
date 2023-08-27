// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ConstructorHelpers.h"
#include "InputCoreTypes.h"
#include "ShowFlags.h"
#include "NavigationData.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "UObject/NameTypes.h"



namespace UtilCommon
{	
	FORCEINLINE bool IsSafe( const UObject* InObject );

	FORCEINLINE bool IsNull( const UObject* InObject );

	template< typename T >
	FORCEINLINE bool IsA( const UObject* InObject );
};

//-----------------------------------------------------------------------------
// inline
FORCEINLINE bool UtilCommon::IsSafe( const UObject* InObject )
{
	return InObject && InObject->IsValidLowLevel() && !InObject->IsPendingKill();
}

FORCEINLINE bool UtilCommon::IsNull( const UObject* InObject )
{
	return UtilCommon::IsSafe( InObject ) == false ;
}

template< typename T >
FORCEINLINE bool UtilCommon::IsA( const UObject* InObject )
{
	if ( UtilCommon::IsSafe( InObject ) )
		return InObject->IsA( T::StaticClass() );

	return false;
}
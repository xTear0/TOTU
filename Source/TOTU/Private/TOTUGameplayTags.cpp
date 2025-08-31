// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "TOTUGameplayTags.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*					PROCESS FOR ADDING NEW ATTRIBUTES					   */
/*-------------------------------------------------------------------------*/
#pragma region New_Attribute_Manual
//		
//		Step (1):
//		In the Attribute Set Header File, start by creating the FGameplay 
//		Attribute Data, with ATTRIBUTE as the name of your attribute:
//			UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ATTRIBUTE, Category = "Attributes")
//			FGameplayAttributeData ATTRIBUTE;
//	 		ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, ATTRIBUTE);
//	
//		Step (2):
//		In the Attribute Set Header File, add the OnRep function for
//		the new attribute:
//			UFUNCTION()
//			void OnRep_ATTRIBUTE(const FGameplayAttributeData& OldATTRIBUTE) const;
//			
//		Step (3):
//		In the Attribute Set CPP File, add the attribute replication settings for the
//		new attribute:
//			DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, ATTRIBUTE, COND_None, REPNOTIFY_Always);
// 
//		Step (4):
//		In the Attribute Set CPP File, add the attribute replication notification for
//		the new attribute:
//			void UTOTUAttributeSet::OnRep_ATTRIBUTE(const FGameplayAttributeData& OldATTRIBUTE) const
//			{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, ATTRIBUTE, OldATTRIBUTE); }
// 
//		Step (5):
//		In the Attribute Set CPP File, add the Tag for the new attribute.
// 			TagsToAttributes.Add(GameplayTags.Attributes_(PRIORITY)_ATTRIBUTE, GetATTRIBUTEAttribute);
// 
//		Step (6):
//		In the GameplayTags Header File, add the Tag for the new attribute.
//			FGameplayTag Attributes_(PRIORITY)_ATTRIBUTE;
// 
//		Step (7):
//		In the GameplayTags CPP File, add the Tag for the new attribute.
//			GameplayTags.Attributes_(PRIORITY)_ATTRIBUTE = UGameplayTagsManager::Get().AddNativeGameplayTag
//				(FName("Attributes.(PRIORITY).ATTRIBUTE"),
//				FString("Does ATTRIBUTE things."));
//
//		Step (8):
//		Check Blueprint Dependancies. I.e. Curve Tables, Data Tables, Message Tables, etc.
//
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                        */
/*-------------------------------------------------------------------------*/
FTOTUGameplayTags FTOTUGameplayTags::GameplayTags; 
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region TOTUGameplayTags.cpp_Functions

#pragma endregion
/*-------------------------------------------------------------------------*/
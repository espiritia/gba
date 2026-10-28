Parte_Secreta_Gimnasio_Plateado_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SecretGym_OnFrame
	.byte 0

@ Recién salido de la cueva: Brock espera debajo del jugador y le propone el trato
SecretGym_OnFrame::
	map_script_2 VAR_PEWTER_CAVE_STATE, 2, SecretGym_EventScript_BrockOffer
	.2byte 0

SecretGym_EventScript_BrockOffer::
	lockall
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_SECRET_GYM_BROCK, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox SecretGym_Text_BrockOffer
SecretGym_EventScript_AskDeal::
	msgbox SecretGym_Text_WhatDoYouSay, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SecretGym_EventScript_DealAccepted
	msgbox SecretGym_Text_Insist
	goto SecretGym_EventScript_AskDeal
	end

SecretGym_EventScript_DealAccepted::
	msgbox SecretGym_Text_Accepted
	closemessage
	setvar VAR_PEWTER_CAVE_STATE, 3
	fadedefaultbgm
	releaseall
	end

SecretGym_EventScript_Brock::
	msgbox SecretGym_Text_Later, MSGBOX_NPC
	end

@ Selector de escenarios de prueba. Solo existe en la ROM de debug (make debug):
@ en el juego se abre desde START > DEBUG.
@
@ Cada escenario:
@   1. limpia el estado de la historia (Debug_Setup_Reset),
@   2. llama al "setup" del punto de la historia que quiere probar. Los setups son
@      acumulativos: cada uno llama primero al anterior, como si se hubiera jugado,
@   3. teletransporta al jugador justo antes del evento.
@
@ Para añadir un escenario: crea un Debug_Setup_X que llame al anterior, un
@ Debug_Scenario_X que lo use y haga warp, añade su "case" abajo y su etiqueta en
@ sListMenuLabels[LISTMENU_DEBUG_SCENARIOS] (src/field_specials.c), en el mismo orden.
@ Si añades flags/vars nuevas de historia, añádelas también a Debug_Setup_Reset.

Debug_EventScript_ScenarioMenu::
	lockall
	setvar VAR_0x8004, LISTMENU_DEBUG_SCENARIOS
	special ListMenu
	waitstate
	switch VAR_RESULT
	case 0, Debug_EventScript_ToolsMenu
	case 1, Debug_Scenario_PewterCubone
	case 2, Debug_Scenario_GymSecretary
	case 3, Debug_Scenario_GymGreen
	case 4, Debug_Scenario_GymBrockQuest
	case 5, Debug_Scenario_ForestCrystal
	case 6, Debug_Scenario_ForestMoss
	case 7, Debug_Scenario_ForestDecoys
	case 8, Debug_Scenario_ForestMew
	case 9, Debug_Scenario_ForestCave
	case 10, Debug_Scenario_BrockBattle
	case 11, Debug_Scenario_AfterBrock
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Escenarios
@ ---------------------------------------------------------------------------

@ Fuera de la casa del Cubone, con Cubone ya recibido
Debug_Scenario_PewterCubone::
	call Debug_Setup_Reset
	call Debug_Setup_GotCubone
	goto Debug_EventScript_WarpPewterHouse2Door

@ En la entrada del gimnasio; al dar un paso salta el test de la secretaria
Debug_Scenario_GymSecretary::
	call Debug_Setup_Reset
	call Debug_Setup_GotCubone
	goto Debug_EventScript_WarpGymEntrance

@ Test superado; Verde (TRAINER_CAMPER_LIAM) sin vencer
Debug_Scenario_GymGreen::
	call Debug_Setup_Reset
	call Debug_Setup_PassedGymQuiz
	special HealPlayerParty
	warp MAP_PEWTER_CITY_GYM, 6, 11
	waitstate
	releaseall
	end

@ Verde vencido; hablar con Brock da el encargo del musgo
Debug_Scenario_GymBrockQuest::
	call Debug_Setup_Reset
	call Debug_Setup_DefeatedGreen
	goto Debug_EventScript_WarpInFrontOfBrock

@ Encargo recibido; al cruzar la columna x=8 suena el cristal
Debug_Scenario_ForestCrystal::
	call Debug_Setup_Reset
	call Debug_Setup_BrockQuest
	special HealPlayerParty
	warp MAP_VIRIDIAN_FOREST, 5, 30
	waitstate
	releaseall
	end

@ Mew ya ha cruzado; primer brillo del rastro en (15,25), dos pasos al norte
Debug_Scenario_ForestMoss::
	call Debug_Setup_Reset
	call Debug_Setup_ForestMossTrail
	special HealPlayerParty
	warp MAP_VIRIDIAN_FOREST, 15, 28
	waitstate
	releaseall
	end

@ Paso 4 del rastro: brillo real en (11,55) y señuelos en (5,60) y (17,56)
Debug_Scenario_ForestDecoys::
	call Debug_Setup_Reset
	call Debug_Setup_ForestMossTrail
	setvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 3
	clearflag FLAG_HIDE_FOREST_MOSS_DECOY1
	clearflag FLAG_HIDE_FOREST_MOSS_DECOY2
	special HealPlayerParty
	warp MAP_VIRIDIAN_FOREST, 11, 59
	waitstate
	releaseall
	end

@ Rastro completo; dos pasos al norte llega la oscuridad, Mew, Agatha y Onix
Debug_Scenario_ForestMew::
	call Debug_Setup_Reset
	call Debug_Setup_ForestMossDone
	special HealPlayerParty
	warp MAP_VIRIDIAN_FOREST, 16, 38
	waitstate
	releaseall
	end

@ Evento de Mew terminado: al principio de la cueva, con Brock esperando más adelante
Debug_Scenario_ForestCave::
	call Debug_Setup_Reset
	call Debug_Setup_ForestMossDone
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 5
	special HealPlayerParty
	warp MAP_GIMNASIO_PLATEADO_PARTE_SECRETA, 5, 4
	waitstate
	releaseall
	end

@ Evento del bosque terminado y combate con Brock desbloqueado
Debug_Scenario_BrockBattle::
	call Debug_Setup_Reset
	call Debug_Setup_BrockUnlocked
	goto Debug_EventScript_WarpInFrontOfBrock

@ Brock vencido, en la puerta del gimnasio (el ayudante de las deportivas espera al este)
Debug_Scenario_AfterBrock::
	call Debug_Setup_Reset
	call Debug_Setup_DefeatedBrock
	special HealPlayerParty
	warp MAP_PEWTER_CITY, 2
	waitstate
	releaseall
	end

Debug_EventScript_WarpPewterHouse2Door::
	special HealPlayerParty
	warp MAP_PEWTER_CITY, 6
	waitstate
	releaseall
	end

Debug_EventScript_WarpGymEntrance::
	special HealPlayerParty
	warp MAP_PEWTER_CITY_GYM, 1
	waitstate
	releaseall
	end

Debug_EventScript_WarpInFrontOfBrock::
	special HealPlayerParty
	warp MAP_PEWTER_CITY_GYM, 6, 7
	waitstate
	releaseall
	end

@ ---------------------------------------------------------------------------
@ Setups acumulativos de la historia (cada uno incluye todo lo anterior)
@ ---------------------------------------------------------------------------

@ Deja la historia personalizada como en una partida nueva
Debug_Setup_Reset::
	special DebugResetParty
	@ Casa del Cubone
	clearflag FLAG_SYS_POKEMON_GET
	clearflag FLAG_GOT_SANDSHREW
	clearflag FLAG_HIDE_SANDSHREW_BALL
	setvar VAR_MAP_SCENE_PEWTER_CITY_HOUSE2, 0
	@ Gimnasio de Plateada
	clearflag FLAG_PASSED_PEWTER_GYM_INTERVIEW
	clearflag FLAG_BROCK_GAVE_MOSS_QUEST
	clearflag FLAG_BROCK_GYM_BATTLE_UNLOCKED
	clearflag FLAG_DEFEATED_BROCK
	clearflag FLAG_BADGE01_GET
	clearflag FLAG_GOT_TM39_FROM_BROCK
	cleartrainerflag TRAINER_CAMPER_LIAM
	cleartrainerflag TRAINER_LEADER_BROCK
	clearflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	setflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	setvar VAR_MAP_SCENE_PEWTER_CITY, 0
	@ Bosque Verde (el resto de flags FLAG_HIDE_* los pone el propio mapa al entrar)
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 0
	setvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 0
	setflag FLAG_HIDE_FOREST_MOSS_DECOY1
	setflag FLAG_HIDE_FOREST_MOSS_DECOY2
	setvar VAR_FOLLOWER_LOCAL_ID, 0
	setvar VAR_FOLLOWER_MAP, 0
	@ Cueva tras el evento de Mew
	setvar VAR_PEWTER_CAVE_STATE, 0
	setvar VAR_PEWTER_CAVE_TIMER_START, 0
	setvar VAR_PEWTER_CAVE_NEXT_QUAKE, 0
	setvar VAR_PEWTER_CAVE_PAUSE_START, 0
	clearflag FLAG_HIDE_PEWTER_CAVE_POTION
	clearflag FLAG_HIDE_PEWTER_CAVE_POKE_BALL
	clearflag FLAG_HIDE_PEWTER_CAVE_ANTIDOTE
	clearflag FLAG_HIDE_PEWTER_CAVE_REPEL
	clearflag FLAG_HIDE_PEWTER_CAVE_BROCK
	clearflag FLAG_GOT_BROCK_GEODUDE
	setrespawn HEAL_LOCATION_PEWTER_CITY
	return

@ Cubone recibido en la casa 2 de Plateada (PewterCity_House2)
Debug_Setup_GotCubone::
	givemon SPECIES_CUBONE, 12, ITEM_MOMS_RICE_CAKES
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_GOT_SANDSHREW
	setflag FLAG_HIDE_SANDSHREW_BALL
	setvar VAR_MAP_SCENE_PEWTER_CITY_HOUSE2, 1
	return

@ Test de la secretaria superado
Debug_Setup_PassedGymQuiz::
	call Debug_Setup_GotCubone
	setflag FLAG_PASSED_PEWTER_GYM_INTERVIEW
	return

@ Verde (TRAINER_CAMPER_LIAM) vencido
Debug_Setup_DefeatedGreen::
	call Debug_Setup_PassedGymQuiz
	settrainerflag TRAINER_CAMPER_LIAM
	return

@ Brock ha dado el encargo del musgo
Debug_Setup_BrockQuest::
	call Debug_Setup_DefeatedGreen
	setflag FLAG_BROCK_GAVE_MOSS_QUEST
	return

@ Bosque: cristal oído y Mew ya ha cruzado; rastro del musgo en el primer brillo
Debug_Setup_ForestMossTrail::
	call Debug_Setup_BrockQuest
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 2
	setvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 0
	return

@ Bosque: los 5 brillos encontrados, a punto de llegar la oscuridad
Debug_Setup_ForestMossDone::
	call Debug_Setup_ForestMossTrail
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 3
	setvar VAR_VIRIDIAN_FOREST_MOSS_TRAIL, 5
	return

@ Bosque terminado y combate con Brock desbloqueado.
@ OJO: en el juego real todavía no hay ningún script que active
@ FLAG_BROCK_GYM_BATTLE_UNLOCKED (el final del bosque es un TODO).
Debug_Setup_BrockUnlocked::
	call Debug_Setup_ForestMossDone
	setvar VAR_MAP_SCENE_VIRIDIAN_FOREST, 5
	setvar VAR_PEWTER_CAVE_STATE, 2
	setflag FLAG_BROCK_GYM_BATTLE_UNLOCKED
	return

@ Brock vencido (lo mismo que PewterCity_Gym_EventScript_DefeatedBrock + MT39)
Debug_Setup_DefeatedBrock::
	call Debug_Setup_BrockUnlocked
	settrainerflag TRAINER_LEADER_BROCK
	setflag FLAG_DEFEATED_BROCK
	setflag FLAG_BADGE01_GET
	setvar VAR_MAP_SCENE_PEWTER_CITY, 1
	setflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	clearflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	set_gym_trainers 1
	additem ITEM_TM39
	setflag FLAG_GOT_TM39_FROM_BROCK
	return

@ ---------------------------------------------------------------------------
@ Herramientas: no reinician nada, se aplican sobre la partida actual
@ ---------------------------------------------------------------------------

Debug_EventScript_ToolsMenu::
	setvar VAR_0x8004, LISTMENU_DEBUG_TOOLS
	special ListMenu
	waitstate
	switch VAR_RESULT
	case 0, Debug_Tool_TravelMenu
	case 1, Debug_Tool_Heal
	case 2, Debug_Tool_StrongTeam
	case 3, Debug_Tool_Money
	case 4, Debug_Tool_Items
	case 5, Debug_Tool_AllBadges
	case 6, Debug_Tool_ModeMenu
	releaseall
	end

@ VIAJAR: aparece delante del Centro Pokémon de la ciudad elegida (coordenadas de
@ src/data/heal_locations.json). No cambia la historia. El orden debe coincidir con
@ sListMenuLabels[LISTMENU_DEBUG_TRAVEL] (src/field_specials.c).
Debug_Tool_TravelMenu::
	setvar VAR_0x8004, LISTMENU_DEBUG_TRAVEL
	special ListMenu
	waitstate
	switch VAR_RESULT
	case 0, Debug_Travel_Pallet
	case 1, Debug_Travel_Viridian
	case 2, Debug_Travel_Pewter
	case 3, Debug_Travel_Cerulean
	case 4, Debug_Travel_Vermilion
	case 5, Debug_Travel_Lavender
	case 6, Debug_Travel_Celadon
	case 7, Debug_Travel_Saffron
	case 8, Debug_Travel_Fuchsia
	case 9, Debug_Travel_Cinnabar
	case 10, Debug_Travel_Indigo
	goto Debug_EventScript_ToolsMenu

Debug_Travel_Pallet::
	warp MAP_PALLET_TOWN, 6, 8
	goto Debug_EventScript_TravelDone

Debug_Travel_Viridian::
	warp MAP_VIRIDIAN_CITY, 26, 27
	goto Debug_EventScript_TravelDone

Debug_Travel_Pewter::
	warp MAP_PEWTER_CITY, 17, 26
	goto Debug_EventScript_TravelDone

Debug_Travel_Cerulean::
	warp MAP_CERULEAN_CITY, 22, 20
	goto Debug_EventScript_TravelDone

Debug_Travel_Vermilion::
	warp MAP_VERMILION_CITY, 15, 7
	goto Debug_EventScript_TravelDone

Debug_Travel_Lavender::
	warp MAP_LAVENDER_TOWN, 6, 6
	goto Debug_EventScript_TravelDone

Debug_Travel_Celadon::
	warp MAP_CELADON_CITY, 48, 12
	goto Debug_EventScript_TravelDone

Debug_Travel_Saffron::
	warp MAP_SAFFRON_CITY, 24, 39
	goto Debug_EventScript_TravelDone

Debug_Travel_Fuchsia::
	warp MAP_FUCHSIA_CITY, 25, 32
	goto Debug_EventScript_TravelDone

Debug_Travel_Cinnabar::
	warp MAP_CINNABAR_ISLAND, 14, 12
	goto Debug_EventScript_TravelDone

Debug_Travel_Indigo::
	warp MAP_INDIGO_PLATEAU_EXTERIOR, 11, 7
	goto Debug_EventScript_TravelDone

Debug_EventScript_TravelDone::
	waitstate
	releaseall
	end

Debug_Tool_Heal::
	special HealPlayerParty
	goto Debug_EventScript_ToolDone

Debug_Tool_StrongTeam::
	givemon SPECIES_CHARIZARD, 50
	givemon SPECIES_BLASTOISE, 50
	givemon SPECIES_VENUSAUR, 50
	setflag FLAG_SYS_POKEMON_GET
	goto Debug_EventScript_ToolDone

Debug_Tool_Money::
	addmoney 50000
	goto Debug_EventScript_ToolDone

Debug_Tool_Items::
	additem ITEM_POKE_BALL, 20
	additem ITEM_SUPER_POTION, 20
	additem ITEM_FULL_RESTORE, 10
	additem ITEM_REVIVE, 10
	additem ITEM_MAX_REPEL, 10
	additem ITEM_ESCAPE_ROPE, 5
	goto Debug_EventScript_ToolDone

Debug_Tool_AllBadges::
	setflag FLAG_BADGE01_GET
	setflag FLAG_BADGE02_GET
	setflag FLAG_BADGE03_GET
	setflag FLAG_BADGE04_GET
	setflag FLAG_BADGE05_GET
	setflag FLAG_BADGE06_GET
	setflag FLAG_BADGE07_GET
	setflag FLAG_BADGE08_GET
	goto Debug_EventScript_ToolDone

@ MODO: NORMAL (juego tal cual) o GAME MASTER, que:
@  - ignora los triggers que impiden el paso (viejo de Ciudad Verde, guía de Plateada,
@    guardias de medallas, garitas…; lista en sDebugPassageBlockScripts,
@    src/field_control_avatar.c) y las salidas tapadas de Ciudad Celeste,
@  - hace al jugador invisible a los entrenadores (CheckForTrainersWantingBattle,
@    src/trainer_see.c): no te paran para luchar, aunque puedes hablarles.
@ Se guarda en FLAG_DEBUG_GAME_MASTER. Debug_Setup_Reset no lo toca.
Debug_Tool_ModeMenu::
	setvar VAR_0x8004, LISTMENU_DEBUG_MODE
	special ListMenu
	waitstate
	switch VAR_RESULT
	case 0, Debug_Tool_ModeNormal
	case 1, Debug_Tool_ModeGameMaster
	goto Debug_EventScript_ToolsMenu

Debug_Tool_ModeNormal::
	clearflag FLAG_DEBUG_GAME_MASTER
	playse SE_SELECT
	msgbox Debug_Text_ModeNormal, MSGBOX_DEFAULT
	closemessage
	goto Debug_EventScript_ToolsMenu

Debug_Tool_ModeGameMaster::
	setflag FLAG_DEBUG_GAME_MASTER
	playse SE_SELECT
	msgbox Debug_Text_ModeGameMaster, MSGBOX_DEFAULT
	closemessage
	goto Debug_EventScript_ToolsMenu

Debug_EventScript_ToolDone::
	playse SE_SELECT
	msgbox Debug_Text_Done, MSGBOX_DEFAULT
	closemessage
	goto Debug_EventScript_ToolsMenu

Debug_Text_Done::
	.string "DEBUG: hecho.$"

Debug_Text_ModeNormal::
	.string "DEBUG: modo NORMAL.$"

Debug_Text_ModeGameMaster::
	.string "DEBUG: modo GAME MASTER.\n"
	.string "Sin bloqueos ni entrenadores.$"

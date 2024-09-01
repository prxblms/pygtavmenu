void PlayMenuSound (const char* Sound)
{
	PLAY_SOUND_FRONTEND(-1, Sound, "HUD_FRONTEND_DEFAULT_SOUNDSET", true);
}

bool IsControlPressed(int button)
{
	bool buttonPress = false;
	if (IS_DISABLED_CONTROL_JUST_PRESSED(0, button))
	{
		buttonPress = true;
	}
	else if (IS_DISABLED_CONTROL_PRESSED(0, button))
	{
		ScrollTimer++;
		if (ScrollTimer > 4)
		{
			buttonPress = true;
			ScrollTimer = 1;
		}
	}
	else if (IS_DISABLED_CONTROL_JUST_RELEASED(0, button))
	{
		ScrollTimer = 0;
	}
	return buttonPress;
}

bool WasXJustPressed()
{
	bool buttonPress = false;
	if (CurrentOption == OptionCount)
	{
		if (!(IS_DISABLED_CONTROL_PRESSED(0, 202) || IS_DISABLED_CONTROL_PRESSED(0, 203)))
		{
			if (IS_DISABLED_CONTROL_JUST_PRESSED(0, 177))
			{
				PlayMenuSound("SELECT");
				buttonPress = true;
			}
		}
	}
	return buttonPress;
}

float GetSafezoneSizeHalved()
{
	float safezoneSize = GET_SAFE_ZONE_SIZE();
	return safezoneSize / 2;
}

void SetMenuPosition(bool LeftSided)
{
	if (LeftSided)
	{
		MenuXCoord1 = 0.5061f - GetSafezoneSizeHalved();
		MenuXCoord2 = 0.7211f - GetSafezoneSizeHalved();
		MenuXCoord3 = 0.6136f - GetSafezoneSizeHalved();
	}
	else
	{
		MenuXCoord1 = 0.2789f + GetSafezoneSizeHalved();
		MenuXCoord2 = 0.4939f + GetSafezoneSizeHalved();
		MenuXCoord3 = 0.3864f + GetSafezoneSizeHalved();
	}
}

void DrawGlare()
{
	GlareHandle = REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
	if (HAS_SCALEFORM_MOVIE_LOADED(GlareHandle))
	{
		DRAW_SCALEFORM_MOVIE(GlareHandle, vector2((MenuXCoord3 + 0.3379f), (0.9647f - GetSafezoneSizeHalved())), Size(1.0f, 1.0f), RGBA(255, 255, 255, 255), 0);
	}
}

void StringBase(int font, int r, int g, int b, bool outline)
{
	SET_TEXT_FONT(font);
	SET_TEXT_SCALE(0.0f, 0.5f);
	SET_TEXT_COLOUR(RGBA(r, g, b, 255));
	SET_TEXT_WRAP(0.0f, MenuXCoord2);
	SET_TEXT_CENTRE(0);
	if (outline)
	{
		SET_TEXT_OUTLINE();
	}
}

bool RequestTexture(const char* TextureDict)
{
	if (!HAS_STREAMED_TEXTURE_DICT_LOADED(TextureDict))
	{
		REQUEST_STREAMED_TEXTURE_DICT(TextureDict, true);
		return false;
	}
	else
	{
		return true;
	}
}

float SmoothScroll(float destinationCoord, float currentCoord)
{
	float newCoord = destinationCoord;
	if (currentCoord != 0.0f)
	{
		if (destinationCoord < currentCoord)
		{
			if (currentCoord - destinationCoord > 0.007f)
			{
				newCoord = currentCoord - (currentCoord - destinationCoord) / 1.7f;
			}
		}
		if (destinationCoord > currentCoord)
		{
			if (destinationCoord - currentCoord > 0.007f)
			{
				newCoord = currentCoord + (destinationCoord - currentCoord) / 1.7f;
			}
		}
	}
	return newCoord;
}

void AddTitle(const char* Title)
{
	StringBase(TitleFont, TitleR, TitleG, TitleB, TextOutlineTitle);
	SET_TEXT_CENTRE(true);
	SET_TEXT_SCALE(0.0f, 0.9f);
	BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(Title);
	END_TEXT_COMMAND_DISPLAY_TEXT(vector2(MenuXCoord3, (0.5346f - GetSafezoneSizeHalved())));
	OptionCount = 0;
}

void DrawBackground(int NumberOfOptions)
{
	float OptionCountFloat;
	float ScrollbarCoordStartingPoint;
	float ScrollbarDestinationCoord;
	SetMenuPosition(LeftSided);
	SubmenuOptionCount = NumberOfOptions;
	if (NumberOfOptions > 13)
	{
		OptionCountFloat = 13.0f;
	}
	else
	{
		OptionCountFloat = NumberOfOptions;
	}
	BackgroundCoord = (OptionCountFloat * 0.035f / 2.0f) + (0.6046f - GetSafezoneSizeHalved());
	BackgroundLength = OptionCountFloat * 0.035f;
	DRAW_RECT(vector2(MenuXCoord3, BackgroundCoord), Size(0.2250f, BackgroundLength), RGBA(BackgroundR, BackgroundG, BackgroundB, BackgroundA));
	// Modloader Lateral Scollbar
	DRAW_RECT(vector2((MenuXCoord3 + 0.2360f), BackgroundCoord), Size(0.010f, BackgroundLength), RGBA(BackgroundR, BackgroundG, BackgroundB, BackgroundA));
	DRAW_RECT(vector2(MenuXCoord3, (0.5596f - GetSafezoneSizeHalved())), Size(0.2250f, 0.09f), RGBA(BannerR, BannerG, BannerB, BannerA));
	if (GlareToggle)
	{
		DrawGlare();
	}
	BottomBarCoord = ((OptionCountFloat + 1.0f) * 0.035f) + (0.5871f - GetSafezoneSizeHalved());
	DRAW_RECT(vector2(MenuXCoord3, BottomBarCoord), Size(0.2250f, 0.035f), RGBA(BannerR, BannerG, BannerB, BannerA));
	if (OptionCountDisplay)
	{
		StringBase(OptionCountFont, OptionCountR, OptionCountG, OptionCountB, TextOutlineOptionCount);
		SET_TEXT_RIGHT_JUSTIFY(true);
		BEGIN_TEXT_COMMAND_DISPLAY_TEXT("CM_ITEM_COUNT");
		ADD_TEXT_COMPONENT_INTEGER(CurrentOption);
		ADD_TEXT_COMPONENT_INTEGER(SubmenuOptionCount);
		END_TEXT_COMMAND_DISPLAY_TEXT(vector2(0.0f, (BottomBarCoord - 0.018f)));
	}
	if (ScrollArrows && (ScrollArrowsAlways || SubmenuOptionCount > 13))
	{
		if (RequestTexture("commonmenu"))
		{
			DRAW_SPRITE("commonmenu", "shop_arrows_upanddown", vector2(MenuXCoord3, BottomBarCoord), Size(0.025f, 0.0444f), 0.0f, RGBA(ScrollArrowsR, ScrollArrowsG, ScrollArrowsB, 255));
		}
	}
	if ((CurrentOption < 7) || (SubmenuOptionCount <= 13))
	{
		ScrollbarCoordStartingPoint = CurrentOption;
	}
	else if ((SubmenuOptionCount - CurrentOption) < 6)
	{
		ScrollbarCoordStartingPoint = 13 - (SubmenuOptionCount - CurrentOption);
	}
	else
	{
		ScrollbarCoordStartingPoint = 7;
	}
	ScrollbarDestinationCoord = (ScrollbarCoordStartingPoint * 0.035f) + (0.5871f - GetSafezoneSizeHalved());
	ScrollArrowsCoord = ScrollbarDestinationCoord;
	if (SmoothScrolling)
	{
		ScrollbarCoord = SmoothScroll(ScrollbarDestinationCoord, ScrollbarCoord);
	}
	else
	{
		ScrollbarCoord = ScrollbarDestinationCoord;
	}
	DRAW_RECT (vector2(MenuXCoord3, ScrollbarCoord), Size(0.2250f, 0.035f), RGBA(ScrollbarR, ScrollbarG, ScrollbarB, ScrollbarA));
}

void DisableControls()
{
	if (DisableSpecialAbility)
	{
		if (IS_SPECIAL_ABILITY_ACTIVE(PLAYER_ID()))
		{
			SPECIAL_ABILITY_DEACTIVATE_FAST(PLAYER_ID());
		}
	}
	if (GET_PED_STEALTH_MOVEMENT(PLAYER_PED_ID()))
	{
		SET_PED_STEALTH_MOVEMENT(PLAYER_PED_ID(), 0, 0);
	}
	setGlobalAtIndex(10434 + 1, 0);
	setGlobalAtIndex(13245 + 4621, GET_GAME_TIMER());
	SET_CINEMATIC_MODE_ACTIVE(0);
	if (LeftSided)
	{
		HIDE_HELP_TEXT_THIS_FRAME();
	}
	HIDE_HUD_COMPONENT_THIS_FRAME(6);
	HIDE_HUD_COMPONENT_THIS_FRAME(7);
	HIDE_HUD_COMPONENT_THIS_FRAME(8);
	HIDE_HUD_COMPONENT_THIS_FRAME(9);
	SET_INPUT_EXCLUSIVE(0, 166);
	SET_INPUT_EXCLUSIVE(0, 167);
	SET_INPUT_EXCLUSIVE(0, 177);
	SET_INPUT_EXCLUSIVE(0, 178);
	SET_INPUT_EXCLUSIVE(0, 202);
	SET_INPUT_EXCLUSIVE(0, 203);
}

RGB GetSpriteColour(bool On)
{
	if (On)
	{
		return RGB(SpriteOnR, SpriteOnG, SpriteOnB);
	}
	else
	{
		return RGB(SpriteOffR, SpriteOffG, SpriteOffB);
	}
}

RGB GetOptionColour()
{
	if (CurrentOption == OptionCount)
	{
		return RGB(SelectedTextR, SelectedTextG, SelectedTextB);
	}
	else
	{
		return RGB(UnselectedTextR, UnselectedTextG, UnselectedTextB);
	}
}

void AddOption(const char* OptionText)
{
	OptionCount++;
	OptionCoordStartingPoint = 0.0f;
	if ((CurrentOption < 7) || (SubmenuOptionCount <= 13))
	{
		if (OptionCount <= 13)
		{
			OptionCoordStartingPoint = OptionCount;
		}
	}
	else if ((SubmenuOptionCount - CurrentOption) < 6)
	{
		if (OptionCount > (SubmenuOptionCount - 13))
		{
			OptionCoordStartingPoint = OptionCount - (SubmenuOptionCount - 13);
		}
	}
	else if ((OptionCount >= (CurrentOption - 6) && (OptionCount <= (CurrentOption + 6))))
	{
		OptionCoordStartingPoint = OptionCount - (CurrentOption - 7);
	}
	if (OptionCoordStartingPoint != 0.0f)
	{
		OptionCoord = (OptionCoordStartingPoint * 0.035f) + (0.5691f - GetSafezoneSizeHalved());
		RGB optionColour = GetOptionColour();
		StringBase(OptionFont, optionColour.r, optionColour.g, optionColour.b, TextOutlineOptions);
		BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(OptionText);
		END_TEXT_COMMAND_DISPLAY_TEXT(vector2(MenuXCoord1, OptionCoord));
	}
}

void AddOnOffOption(bool On)
{
	if (OptionCoordStartingPoint != 0.0f)
	{
		SpriteCoord = (OptionCoordStartingPoint * 0.035f) + (0.5871f - GetSafezoneSizeHalved());
		if (RequestTexture("commonmenu"))
		{
			if (SpriteOutline)
			{
				DRAW_SPRITE("commonmenu", "common_medal", vector2((MenuXCoord2 - 0.006f), SpriteCoord), Size(0.03f, 0.0494f), 0.0f, RGBA(SpriteOutlineR, SpriteOutlineG, SpriteOutlineB, 255));
			}
			RGB spriteColour = GetSpriteColour(On);
			DRAW_SPRITE("commonmenu", "common_medal", vector2((MenuXCoord2 - 0.006f), SpriteCoord), Size(0.025f, 0.0444f), 0.0f, RGBA(spriteColour.r, spriteColour.g, spriteColour.b, 255));
		}
	}
}

void GetTextWrap()
{
	if (CurrentOption == OptionCount)
	{
		SET_TEXT_WRAP(0.0f, (MenuXCoord2 - 0.031f));
	}
	else
	{
		SET_TEXT_WRAP(0.0f, (MenuXCoord2 - 0.014f));
	}
}

int GetStackSizeFromValue(int value)
{
	int StackSize;
	switch(value)
	{
		case 0: {StackSize = 128; break;}
		case 1: {StackSize = 512; break;}
		case 2: {StackSize = 1024; break;}
		case 3: {StackSize = 1820; break;}
		case 4: {StackSize = 1828; break;}
		case 5: {StackSize = 2024; break;}
		case 6: {StackSize = 2552; break;}
		case 7: {StackSize = 2556; break;}
		case 8: {StackSize = 2800; break;}
		case 9: {StackSize = 3076; break;}
		case 10: {StackSize = 3150; break;}
		case 11: {StackSize = 3584; break;}
		case 12: {StackSize = 4592; break;}
		case 13: {StackSize = 4600; break;}
		case 14: {StackSize = 5000; break;}
		case 15: {StackSize = 6304; break;}
		case 16: {StackSize = 8032; break;}
		case 17: {StackSize = 9300; break;}
		case 18: {StackSize = 15000; break;}
		case 19: {StackSize = 21000; break;}
	}
	return StackSize;
}

void AddLeftRightOption()
{
	if (RequestTexture("commonmenu"))
	{
		DRAW_SPRITE("commonmenu", "shop_arrows_upanddown", vector2((MenuXCoord2 - 0.0224f), ScrollArrowsCoord), Size(0.025f, 0.0444f), 90.0f, RGBA(SelectedTextR, SelectedTextG, SelectedTextB, 255));
	}
}

void AddIntOption(int* DisplayNumber, int minValue, int maxValue)
{
	if (CurrentOption == OptionCount)
	{
		AddLeftRightOption();
		if (IsControlPressed(166))
		{
			PlayMenuSound("NAV_LEFT_RIGHT");
			if (*DisplayNumber == minValue)
			{
				*DisplayNumber = maxValue;
			}
			else
			{
				*DisplayNumber = *DisplayNumber - 1;
			}
		}
		else if (IsControlPressed(167))
		{
			PlayMenuSound("NAV_LEFT_RIGHT");
			if (*DisplayNumber == maxValue)
			{
				*DisplayNumber = minValue;
			}
			else
			{
				*DisplayNumber = *DisplayNumber + 1;
			}
		}
	}
	if (OptionCoordStartingPoint != 0.0f)
	{
		RGB optionColour = GetOptionColour();
		StringBase(OptionFont, optionColour.r, optionColour.g, optionColour.b, TextOutlineOptions);
		GetTextWrap();
		SET_TEXT_RIGHT_JUSTIFY(true);
		BEGIN_TEXT_COMMAND_DISPLAY_TEXT("NUMBER");
		ADD_TEXT_COMPONENT_INTEGER(GetStackSizeFromValue(*DisplayNumber));
		END_TEXT_COMMAND_DISPLAY_TEXT(vector2(0.0f, OptionCoord));
	}
}

void DrawMenuOpeningBinds(int Button1, int Button2)
{
	if (OptionCoordStartingPoint != 0.0f)
	{
		SpriteCoord = (OptionCoordStartingPoint * 0.035f) + (0.5871f - GetSafezoneSizeHalved());
		if (RequestTexture("erootiik"))
		{
			if (Button1 != 0)
			{
				DRAW_SPRITE("erootiik", straddiGlobal("button_", Button1), vector2((MenuXCoord2 - 0.066f), SpriteCoord), Size(0.016f, 0.0289f), 0.0f, RGBA(255, 255, 255, 255));
				if (Button2 != 0)
				{
					StringBase(0, 255, 255, 255, true);
					SET_TEXT_SCALE(0.0f, 0.435f);
					SET_TEXT_CENTRE(1);
					BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
					ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("+");
					END_TEXT_COMMAND_DISPLAY_TEXT(vector2((MenuXCoord2 - 0.051f), OptionCoord - 0.0007f));
				}
			}
			if (Button2 != 0)
			{
				DRAW_SPRITE("erootiik", straddiGlobal("button_", Button2), vector2((MenuXCoord2 - 0.036f), SpriteCoord), Size(0.016f, 0.0289f), 0.0f, RGBA(255, 255, 255, 255));
			}
		}
	}
}

void AddScriptOption(const char* OptionText, char* ScriptName, int StackSize, int Button1, int Button2)
{
	char* ErrorMessage;
	if (!DOES_SCRIPT_EXIST(ScriptName))
	{
		if (IS_PS3_VERSION())
		{
			ErrorMessage = ".csc Not Found!";
		}
		else
		{
			ErrorMessage = ".xsc Not Found!";
		}
		AddOption(strcatGlobal(ScriptName, ErrorMessage));
	}
	else
	{
		AddOption(OptionText);
		DrawMenuOpeningBinds(Button1, Button2);
		AddOnOffOption(_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(getHashKey(ScriptName)) != 0);
		if (WasXJustPressed())
		{
			if (_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(getHashKey(ScriptName)) == 0)
			{
				ScriptToLoad = ScriptName;
				ScriptToLoadStackSize = StackSize;
			}
			else
			{
				TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(ScriptName);
			}
		}
	}
}

void AddInstructionalButton(int Button, const char* Text)
{
	if (!AlreadySetup)
	{
		InstructionalButtonsHandle = REQUEST_SCALEFORM_MOVIE("INSTRUCTIONAL_BUTTONS");
		if (HAS_SCALEFORM_MOVIE_LOADED(InstructionalButtonsHandle))
		{
			DRAW_SCALEFORM_MOVIE_FULLSCREEN(InstructionalButtonsHandle, RGBA(255, 255, 255, 255), 0);
			_PUSH_SCALEFORM_MOVIE_FUNCTION(InstructionalButtonsHandle, "CLEAR_ALL");
			_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
			_PUSH_SCALEFORM_MOVIE_FUNCTION(InstructionalButtonsHandle, "SET_CLEAR_SPACE");
			_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(200);
			_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
			InstructionalButtonCount = -1;
			AlreadySetup = true;
			goto AddInstructionalButtonContinue;
		}
	}
	else
	{
		AddInstructionalButtonContinue:
		InstructionalButtonCount++;
		_PUSH_SCALEFORM_MOVIE_FUNCTION(InstructionalButtonsHandle, "SET_DATA_SLOT");
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(InstructionalButtonCount);
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(Button);
		BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(Text);
		END_TEXT_COMMAND_SCALEFORM_STRING();
		_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
	}
}

void DrawInstructionalButtons()
{
	if (AlreadySetup)
	{
		_PUSH_SCALEFORM_MOVIE_FUNCTION(InstructionalButtonsHandle, "DRAW_INSTRUCTIONAL_BUTTONS");
		_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		_PUSH_SCALEFORM_MOVIE_FUNCTION(InstructionalButtonsHandle, "SET_BACKGROUND_COLOUR");
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(80);
		_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
		AlreadySetup = false;
	}
}

void AddScriptOptionNoBinds(char* ScriptName, int* StackSize)
{
	if(!(IS_STRING_NULL_OR_EMPTY(ScriptName)))
	{
		AddOption(ScriptName);
		if (CurrentOption == OptionCount)
		{
			AddInstructionalButton(32, "Remove");
			SET_INPUT_EXCLUSIVE(0, 179);
			if (IS_DISABLED_CONTROL_JUST_PRESSED(0, 179))
			{
				strcpy(ScriptName, "", 31);
				*StackSize = 0;
				CustomScriptCount--;
				if (CurrentOption != 1)
				{
					CurrentOption--;
				}
			}
		}
		if (DOES_SCRIPT_EXIST(ScriptName))
		{
			AddOnOffOption(_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(getHashKey(ScriptName)) != 0);
			AddIntOption(StackSize, 0, 19);
			if (WasXJustPressed())
			{
				if (_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(getHashKey(ScriptName)) == 0)
				{
					ScriptToLoad = ScriptName;
					ScriptToLoadStackSize = GetStackSizeFromValue(*StackSize);
				}
				else
				{
					TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(ScriptName);
				}
			}
		}
	}
}

void AddSubmenuOption(const char* OptionText, void* submenuLoc)
{
	AddOption(OptionText);
	if (WasXJustPressed())
	{
		LastSubmenu[SubmenuLevel] = CurrentSubmenu;
		LastOption[SubmenuLevel] = CurrentOption;
		CurrentSubmenu = submenuLoc;
		CurrentOption = 1;
		SubmenuLevel++;
	}
}

void AddCustomScriptOption()
{
	AddOption("Add Custom Script");
	if (WasXJustPressed())
	{
		DISPLAY_ONSCREEN_KEYBOARD(0, "", "", "", "", "", "", 31);
		KeyboardMonitoring = true;
	}
}

void CustomScripts()
{
	AddScriptOptionNoBinds(CustomScript1, &CustomScript1StackSize);
	AddScriptOptionNoBinds(CustomScript2, &CustomScript2StackSize);
	AddScriptOptionNoBinds(CustomScript3, &CustomScript3StackSize);
	AddScriptOptionNoBinds(CustomScript4, &CustomScript4StackSize);
	AddScriptOptionNoBinds(CustomScript5, &CustomScript5StackSize);
	AddScriptOptionNoBinds(CustomScript6, &CustomScript6StackSize);
	AddScriptOptionNoBinds(CustomScript7, &CustomScript7StackSize);
	AddScriptOptionNoBinds(CustomScript8, &CustomScript8StackSize);
	AddScriptOptionNoBinds(CustomScript9, &CustomScript9StackSize);
	AddScriptOptionNoBinds(CustomScript10, &CustomScript10StackSize);
	if (AddCustomScript)
	{
		AddCustomScriptOption();
	}
	if (AlreadySetup)
	{
		DrawInstructionalButtons();
	}
}

void SetKeyboardResultIntoOption(const char* ScriptName)
{
	if (IS_STRING_NULL_OR_EMPTY(CustomScript1))
	{
		strcpy(CustomScript1, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript2))
	{
		strcpy(CustomScript2, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript3))
	{
		strcpy(CustomScript3, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript4))
	{
		strcpy(CustomScript4, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript5))
	{
		strcpy(CustomScript5, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript6))
	{
		strcpy(CustomScript6, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript7))
	{
		strcpy(CustomScript7, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript8))
	{
		strcpy(CustomScript8, ScriptName, 31);
	}
	else if (IS_STRING_NULL_OR_EMPTY(CustomScript9))
	{
		strcpy(CustomScript9, ScriptName, 31);
	}
	else
	{
		strcpy(CustomScript10, ScriptName, 31);
	}
}

void AutoLoadScript(char* ScriptName, int StackSize)
{
	if (DOES_SCRIPT_EXIST(ScriptName))
	{
		if (_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(getHashKey(ScriptName)) == 0)
		{
			HasScriptLoaded:
			if (!HAS_SCRIPT_LOADED(ScriptName))
			{
				REQUEST_SCRIPT(ScriptName);
				WAIT(0);
				goto HasScriptLoaded;
			}
			START_NEW_SCRIPT(ScriptName, StackSize);
			SET_SCRIPT_AS_NO_LONGER_NEEDED(ScriptName);
		}
	}
}

void DrawBindHint(const char* text, int Button1, int Button2, int font, bool background, bool pulse)
{
	float textWidth;
	if (RequestTexture("erootiik"))
	{
		StringBase(font, 255, 255, 255, true);
		_BEGIN_TEXT_COMMAND_WIDTH("STRING");
		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		textWidth = _END_TEXT_COMMAND_GET_WIDTH(true) + 0.046f;//width of buttons
		if (background)
		{
			DRAW_RECT(vector2(0.5f, (0.5326f - GetSafezoneSizeHalved())), Size(textWidth, 0.035f), RGBA(0, 0, 0, 125));
		}
		if (pulse)
		{
			if (PulseStage == 0)
			{
				if (Alpha >= 5)
				{
					Alpha = Alpha - 5;
				}
				else
				{
					PulseStage = 1;
				}
			}
			else
			{
				if (Alpha <= 250)
				{
					Alpha = Alpha + 5;
				}
				else
				{
					PulseStage = 0;
				}
			}
		}
		StringBase(font, 255, 255, 255, true);
		SET_TEXT_COLOUR(RGBA(255, 255, 255, Alpha));
		BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		END_TEXT_COMMAND_DISPLAY_TEXT(vector2((0.5007f - (textWidth / 2)), (0.5146f - GetSafezoneSizeHalved())));
		DRAW_SPRITE("erootiik", straddiGlobal("button_", Button1), vector2(0.4607f + (textWidth / 2), (0.5326f - GetSafezoneSizeHalved())), Size(0.016f, 0.0289f), 0.0f, RGBA(255, 255, 255, Alpha));
		StringBase(0, 255, 255, 255, true);
		SET_TEXT_SCALE(0.0f, 0.435f);
		SET_TEXT_COLOUR(RGBA(255, 255, 255, Alpha));
		SET_TEXT_CENTRE(1);
		BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("+");
		END_TEXT_COMMAND_DISPLAY_TEXT(vector2(0.4757f + (textWidth / 2), (0.5139f - GetSafezoneSizeHalved())));
		DRAW_SPRITE("erootiik", straddiGlobal("button_", Button2), vector2(0.4907f + (textWidth / 2), (0.5326f - GetSafezoneSizeHalved())), Size(0.016f, 0.0289f), 0.0f, RGBA(255, 255, 255, Alpha));
	}
}
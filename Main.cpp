#include <Windows.h>
#include "Class.h"
#include "Offset.h"

auto X = 800, Y = 600, B = 32;
float SizeX, SizeY;
bool Debug, misc1, outpost, skelles, misc2;

int Draw(DWORD_PTR ULevel) {
	auto AController = Read<DWORD_PTR>(UPlayer + PlayerController);
	auto CameraManager = Read<DWORD_PTR>(AController + PlayerCameraManager);
	auto APawn = Read<DWORD_PTR>(AController + Pawn);
	auto ActorArray = Read<TArray>(ULevel + LevelArray);

	for (int i = 0; i < ActorArray.Count; ++i) {
		auto Actor = Read<DWORD_PTR>(ActorArray.Data + i * sizeof(DWORD_PTR));

		if (!Actor)
			continue;

		if (Actor == APawn)
			continue;

		auto ObjectIndex = Read<DWORD>(Actor + ComparisonIndex);
		auto ElementsPerChunk = 16 * 1024;
		auto NamesIndex = Read<DWORD_PTR>(GNames + ObjectIndex / ElementsPerChunk * sizeof(DWORD_PTR));
		auto NamesPointer = Read<DWORD_PTR>(NamesIndex + ObjectIndex % ElementsPerChunk * sizeof(DWORD_PTR));


		string ActorName = Read<FNameEntry>(NamesPointer).AnsiName;

		auto ActorRootComponent = Read<DWORD_PTR>(Actor + RootComponent);
		string hash = to_string(Read<DWORD_PTR>(ActorRootComponent + ComparisonIndex));

		if (ActorName.find("") == -1)
			continue;

		auto Name = ActorName+" / " + hash;
		auto Color = Shipwreck;

		/* --------- Items BP's --------- */

		// Debug
		if (Debug) {

		// Ships 
		// Sloop
		}
		 else if (ActorName.find("BP_SmallShipTemplate_C") != -1 || ActorName.find("BP_SmallShipNetProxy_C") != -1) {
			Name = "Sloop";
			Color = Ship;
			if (ActorName.find("NetProxy") != -1)
				Name = "Sloop";

		// Brigantine
		}
		 else if (ActorName.find("BP_MediumShipTemplate_C") != -1 || ActorName.find("BP_MediumShipNetProxy_C") != -1) {
			Name = "Brigantine";
			Color = Ship;
			if (ActorName.find("NetProxy") != -1)
				Name = "Brigantine";

		 // Galleon
		 }
		 else if (ActorName.find("BP_LargeShipTemplate_C") != -1 || ActorName.find("BP_LargeShipNetProxy_C") != -1) {
			Name = "Galleon";
			Color = Ship;
		    if (ActorName.find("NetProxy") != -1)
				Name = "Galleon";
                Color = Ship;


		// Ghostship
		 }
		 else if (ActorName.find("BP_AILargeShipTemplate_C") != -1 || ActorName.find("BP_AILargeShipNetProxy_C") != -1) {
			Name = "GhostShip";
			Color = Ghostship;
		    if (ActorName.find("NetProxy") != -1)
				Name = "GhostShip";
                Color = Ghostship;


		// Player
		}else if (ActorName.find("PlayerPirate") != -1) {
			auto PlayerInfo = Read<DWORD_PTR>(Actor + PlayerState);
			auto PlayerIndex = Read<DWORD_PTR>(PlayerInfo + PlayerName);
			wstring PlayerString = Read<FString>(PlayerIndex).WideName;
			Name = string(PlayerString.begin(), PlayerString.end());
			Color = Player;

		// Cannonballs
		}else if (ActorName.find("BP_Projectile_CannonBall_C") != -1) {
			Name = "Cannonball";
			Color = Cannonball;

		// Outposts
		}else if (outpost && ActorName.find("Copiedwld_outpost") != -1) {
			Name = "Outpost";
			Color = Outpost;
		}
		else if (outpost && ActorName.find("Copiedwsp_outpost") != -1) {
			Name = "Outpost";
			Color = Outpost;
		}
		else if (outpost && ActorName.find("Copiedbsp_outpost") != -1) {
			Name = "Outpost";
			Color = Outpost;
		}
		else if (outpost && ActorName.find("Copieddvr_outpost") != -1) {
			Name = "Outpost";
			Color = Outpost;

		// Message in a bottle
		}
		else if (ActorName.find("BP_MessageInABottleProxy_C") != -1) {
			Name = "Message";
			Color = Mermaid;

		// Kraken
		}
		else if (ActorName.find("BP_Kraken_C") != -1) {
			Name = "Kraken";
			Color = Kraken;
 
		// Rowboat
		}else if (ActorName.find("BP_RowboatRowingSeat_C") != -1) {
			Name = "Rowboat";
			Color = Rowboat;

		// Megalodon
		}else if (ActorName.find("BP_TinyShark_C") != -1) {
			Name = "Megalodon";
			Color = Megalodon;

		// Mermaid
		}
		else if (ActorName.find("BP_Mermaid_C") != -1) {
			Name = "Mermaid";
			Color = Mermaid;

		// Skeleton Fort
		}
		else if (ActorName.find("SkellyFort") != -1) {
			Color = Cloud;
			Name = "Skeleton Fort";

			// Ghost ship cloud
		}
		else if (ActorName.find("BP_SkellyShip_ShipCloud_C") != -1) {
			Color = Cloud;
			Name = "Skeleton Ship ";

			// Shipwrecks
		}
		else if (ActorName.find("Shipwreck_01_a_NetProxy") != -1) {
			Name = "Shipwreck";
			Color = Shipwreck;

			// Skeleton
		}
		else if (skelles && ActorName.find("Skeleton") != -1 && ActorName.find("Pawn") != -1) {
			Name = "Skeleton";
			Color = Enemy;

		// Chests
		}
		else if (misc1 && ActorName.find("TreasureChest") != -1 && ActorName.find("ItemInfo") == -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Castaways Chest";
				Color = Common;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Castaways Chest";
			}
			else if (ActorName.find("Rare") != -1) {
				Name = "Seafarers Chest";
				Color = Rare;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Seafarers Chest";
			}
			else if (ActorName.find("Legendary") != -1) {
				Name = "Marauders Chest";
				Color = Legendary;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Marauders Chest";
			}
			else if (ActorName.find("Mythical") != -1) {
				Name = "Captains Chest";
				Color = Mythical;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Captains Chest";
			}
			else if (ActorName.find("PirateLegend") != -1) {
				Name = "Chest of Legends";
				Color = Mythical;
				if (ActorName.find("DVR") != -1)
					Name = "Ashen Chest of Legends";
			}
			else if (ActorName.find("Drunken") != -1) {
				Name = "Chest of Thousand Grogs";
				Color = Mythical;
			}
			else if (ActorName.find("Weeping") != -1) {
				Name = "Chest of Sorrows";
				Color = Mythical;
			}
			else if (ActorName.find("Fort") != -1) {
				Name = "Stronghold Chest";
				Color = Mythical;
			}

		// Merchant crates
		}
		else if (misc1 && ActorName.find("MerchantCrate") != -1 && ActorName.find("ItemInfo") == -1) {
			if (ActorName.find("SugarCrate") != -1) {
				Name = "SugarCrate";
				Color = Common;
			}
			else if (ActorName.find("MerchantCrate_GunpowderBarrelProxy_C") != -1) {
				Name = "Gunpowder";
				Color = Gunpowder;
			}
			else if (ActorName.find("MerchantCrate_GunpowderBarrel_C") != -1) {
				Name = "Gunpowder";
				Color = Gunpowder;
			}
			else if (ActorName.find("BP_MerchantCrate_Commodity_VolcanicStone_Proxy_C") != -1) {
				Name = "Volcanic Stone";
				Color = Common;
			}
			else if (ActorName.find("Tea") != -1) {
				Name = "Tea Crate";
				Color = Rare;
			}
			else if (ActorName.find("Spice") != -1) {
				Name = "Spice Crate";
				Color = Rare;
			}
			else if (ActorName.find("BP_MerchantCrate_BigGunpowderBarrelProxy_C") != -1) {
				Name = "StrongholdGunpower";
				Color = Mythical;
			}
			else if (ActorName.find("BP_MerchantCrate_Commodity_SilkCrate_Proxy_C") != -1) {
				Name = "Silk Crate";
				Color = Legendary;
			}
			else if (ActorName.find("BP_MerchantCrate_Commodity_Minerals_Proxy_C") != -1) {
				Name = "Minerals Crate";
				Color = Common;
			}
			else if (ActorName.find("BP_MerchantCrate_Commodity_Fort_Proxy_C") != -1) {
				Name = "Stronghold Crate";
				Color = Mythical;
			}
			else if (ActorName.find("BP_MerchantCrate_WoodCrateProxy_C") != -1) {
				Name = "Wood Crate";
				Color = Rare;
			}
			else if (ActorName.find("SnakeBasket") != -1) {
				Name = "SnakeBasket";
				Color = Common;
			}
			else if (ActorName.find("ChickenCrate") != -1) {
				Name = "ChickenCrate";
				Color = Common;
			}
			else if (ActorName.find("PigCrate") != -1) {
				Name = "PigCrate";
				Color = Common;
			}


	    // Cargo Run Crates
		}
		else if (misc1 && ActorName.find("CargoRunCrate") != -1 && ActorName.find("ItemInfo") == -1) {
		   if (ActorName.find("Cloth_Proxy") != -1) {
			Name = "Luxurious Cloth";
			Color = Rare;
		}
		else if (ActorName.find("Rum_Proxy") != -1) {
			Name = "Rum Bottles";
			Color = Rare;
		   }
		else if (ActorName.find("Plants_Proxy") != -1) {
			   Name = "Plants";
			   Color = Rare;
		}


		// Chickens
		}
		else if (misc2 && ActorName.find("BP_Chicken") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "White Chicken";
				Color = Common;
			}
			else if (ActorName.find("Rare") != -1) {
				Name = "Red Speckled Chicken";
				Color = Rare;
			}
			else if (ActorName.find("Legendary") != -1) {
				Name = "Black Plumed Chicken";
				Color = Legendary;
			}
			else if (ActorName.find("Mythical") != -1) {
				Name = "Golden Chicken";
				Color = Mythical;
			}

		// Pigs
		}
		else if (misc2 && ActorName.find("BP_Pig") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Pink Pig";
				Color = Common;
			}
			else if (ActorName.find("Rare") != -1) {
				Name = "Black Spotted Pig";
				Color = Rare;
			}
			else if (ActorName.find("Legendary") != -1) {
				Name = "Black Coated Pig";
				Color = Legendary;
			}
			else if (ActorName.find("Mythical") != -1) {
				Name = "Gold Striped Pig";
				Color = Mythical;
			}

		// Snakes
		}
		else if (misc2 && ActorName.find("BP_Snake") != -1) {
			if (ActorName.find("Common") != -1) {
				Name = "Red Striped Snake";
				Color = Common;
			}
			else if (ActorName.find("Rare") != -1) {
				Name = "Blue Dappled Snake";
				Color = Rare;
			}
			else if (ActorName.find("Legendary") != -1) {
				Name = "Black Scaled Snake";
				Color = Legendary;
			}
			else if (ActorName.find("Mythical") != -1) {
				Name = "Golden Snake";
				Color = Mythical;
			}

        
		//Artifacts
		}
		else if (misc1 && ActorName.find("BP_Treasure_Artifact") != -1 && ActorName.find("ItemInfo") == -1) {
		if (ActorName.find("vase") != -1) {
			Name = "Shiny Vase";
			Color = Common;
			if (ActorName.find("DVR") != -1) {
				Name = "Ashen Shiny Vase";
			}
		}
		else if (ActorName.find("box") != -1) {
			Name = "Shiny Box";
			Color = Common;
			if (ActorName.find("DVR") != -1) {
				Name = "Ashen Shiny Box";
			}
		}
		else if (ActorName.find("goblet") != -1) {
			Name = "Shiny Goblet";
			Color = Common;
			if (ActorName.find("DVR") != -1) {
				Name = "Ashen Shiny Goblet";
			}
		}
		else if (ActorName.find("impressive") != -1) {
			Name = "Shiny Impressive";
			Color = Mythical;
			if (ActorName.find("DVR") != -1) {
				Name = "Ashen Shiny Impressive";
			}
		}

        

         //Gem Mermaids
		}
		else if (misc1 && ActorName.find("SunkenCurse") != -1) {
		if (ActorName.find("Ruby") != -1) {
			Name = "Ruby Underwater Statue";
			Color = Ghostship;	
		}
		else if (ActorName.find("Emerald") != -1) {
			Name = "Emerald Underwater Statue";
			Color = Emerald;
		}
		else if (ActorName.find("Sapphire") != -1) {
			Name = "Sapphire Underwater Statue";
			Color = Sapphire;
           }

		// Skulls
        }
		else if (misc1 && ActorName.find("BountyRewardSkull") != -1 && ActorName.find("Proxy") != -1 || ActorName.find("BP_BountRewardSkull") != -1 && ActorName.find("ItemInfo") == -1) {
        if (ActorName.find("Mythical") != -1) {
			Name = "Villainous Skull";
			Color = Mythical;
			if (ActorName.find("BP_BountyRewardSkull_Legendary_C") != -1)
				Name = "Villainous Skull";
			if (ActorName.find("DVR") != -1)
				Name = "Ashens Villainous Skull";
		}
		else if (ActorName.find("Legendary") != -1) {
			Name = "Hatefull Skull";
			Color = Legendary;
			if (ActorName.find("DVR") != -1)
				Name = "Ashen Hatefull Skull";
		}
		else if (ActorName.find("Rare") != -1) {
			Name = "Disgraced Skull";
			Color = Rare;
			if (ActorName.find("DVR") != -1)
				Name = "Ashen Disgraced Skull";
		}
		else if (ActorName.find("Common") != -1) {
			Name = "Foul Skull";
			Color = Common;
			if (ActorName.find("DVR") != -1)
				Name = "Ashen Foul Skull";
		}
		else if (ActorName.find("Fort") != -1) {
			Name = "Stronghold Skull";
			Color = Mythical;
		}
		else if (ActorName.find("AIShip_C") != -1) {
			Name = "Skeleton Captain Skull";
			Color = Mythical;
		}


		} else {
			continue;
		}



		/* ---------------------------------*/

		//auto ActorRootComponent = Read<DWORD_PTR>(Actor + RootComponent);
		auto ActorComponent = Read<FTransform>(ActorRootComponent + ComponentToWorld);
		auto PlayerCamera = Read<FMinimalViewInfo, DWORD_PTR>(CameraManager + CameraCache + POV);
        auto Location = ActorComponent.Translation - PlayerCamera.Location;
		auto Distance = sqrt(Location.Dot(Location)) / 100;



        //Ships
		 if (Distance > 1041 && (ActorName.find("BP_SmallShipTemplate_C") != -1 || ActorName.find("BP_MediumShipTemplate_C") != -1 || ActorName.find("BP_LargeShipTemplate_C") != -1))
         {  Name=""; }
		else
	     {Name += " [" + to_string((int)sqrtf(Location.Dot(Location)) / 100) + "m]"; }

		 //Ghost ship
		 if (Distance > 1050 && (ActorName.find("BP_AILargeShipTemplate_C") != -1))
		 {Name = "";}

		 //Players
		 if (Distance > 450 && (ActorName.find("BP_PlayerPirate_C") != -1))
		 {Name = "";}

		 //RowBoat
		 if (Distance > 600 && (ActorName.find("BP_RowboatRowingSeat_C") != -1))
		 {Name = "";}

		 //Cannon Balls
		 if (Distance > 700 && (ActorName.find("BP_Projectile_CannonBall_C") != -1))
		 {Name = "";}


        wstring NameWide(Name.begin(), Name.end());


		auto Project = WorldToScreen(Location, PlayerCamera, SizeX, SizeY);
		RenderTarget->CreateSolidColorBrush(Color, &Brush);
		RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0x000000, 1.0f)), &BlackBrush);
		RenderTarget->SetTransform(Matrix3x2F::Translation(Project.X, Project.Y));
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 1, 1), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 1, -1), BlackBrush);	
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 1, 0), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 0, -1), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 0, 1), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, -1, 1), BlackBrush);
        RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, -1, 0), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, -1, -1), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, -1, 0), BlackBrush);
		RenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), TextFormat, RectF((float)X, (float)Y, 0, 0), Brush);

	}

	return 0;
}

int Render(HWND Window) {
	auto Target = FindWindow(0, GameName);
	if (Target) {
		DwmGetWindowAttribute(Target, DWMWA_EXTENDED_FRAME_BOUNDS, &Frame, sizeof(Frame));
		X = Frame.right - Frame.left;
		Y = Frame.bottom - Frame.top - B;
		MoveWindow(Window, Frame.left, Frame.top + B, X, Y, true);

		SizeX = (float)X / 2.f;
		SizeY = (float)Y / 2.f;
	}

	RenderTarget->Resize(SizeU(X, Y));
	RenderTarget->BeginDraw();
	RenderTarget->Clear();

	if (!UEngine) {
		Setup();
	}
	else if (Read<DWORD_PTR>(UEngine + ViewportClient)) {
		if (!UPlayer) {
			UViewport = Read<DWORD_PTR>(UEngine + ViewportClient);
			auto UGameInstance = Read<DWORD_PTR>(UViewport + GameInstance);
			auto ULocalPlayer = Read<DWORD_PTR>(UGameInstance + LocalPlayers);
			UPlayer = Read<DWORD_PTR>(ULocalPlayer);
		}

		if (Read<DWORD_PTR>(UViewport + World)) {
			auto UWorld = Read<DWORD_PTR>(UViewport + World);
			Draw(Read<DWORD_PTR>(UWorld + PersistentLevel));
		}
	}


	// Crosshair
	RenderTarget->CreateSolidColorBrush(Common, &Brush);
	RenderTarget->SetTransform(Matrix3x2F::Identity());
	RenderTarget->DrawLine(Point2F(SizeX + 8.5f, SizeY), Point2F(SizeX - 8.5f, SizeY), Brush, 1.5f);
	RenderTarget->DrawLine(Point2F(SizeX, SizeY + 8.f), Point2F(SizeX, SizeY - 8.5f), Brush, 1.5f);

	RenderTarget->EndDraw();
	Sleep(5);


	// Animals Toggle = F6
	if (GetAsyncKeyState(VK_F6) & 1) {
		if (misc2)
			misc2 = false;
		else
			misc2 = true;


	// Loot Toggle = F5
	}else if (GetAsyncKeyState(VK_F5) & 1) {
		if (misc1)
			misc1 = false;
		else
			misc1 = true;


	// Outposts Toggle = F4
	}
	else if (GetAsyncKeyState(VK_F4) & 1) {
		if (outpost)
			outpost = false;
		else
			outpost = true;

	// Debug = F8
	}
	else if (GetAsyncKeyState(VK_F7) & 1) {
		if (skelles)
			skelles = false;
		else
			skelles = true;


	// Debug = F8
	}else if (GetAsyncKeyState(VK_F8) & 1) {
		if (Debug)
			Debug = false;
		else
			Debug = true;


	// Exit = F12
	}
	else if (GetAsyncKeyState(VK_F12) & 1) {
		exit(1);
	}

	return 0;
}

 LRESULT CALLBACK WinProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_CREATE:
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
		auto RenderProperties = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		Factory->CreateHwndRenderTarget(RenderProperties, HwndRenderTargetProperties(Window, SizeU(X, Y)), &RenderTarget);
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&DWriteFactory);
			DWriteFactory->CreateTextFormat(L"Tohoma", 0, DWRITE_FONT_WEIGHT_MEDIUM, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_CONDENSED, 13, L"en-us", &TextFormat);
			return 0;

	case WM_PAINT:
		Render(Window);
		return 0;

	case WM_DESTROY:
		exit(1);
	}
	return DefWindowProc(Window, Message, wParam, lParam);
}
 //Source made by simplyHacking
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE, LPSTR, INT) {
	MSG Message;
	MARGINS Margin = { -1 };
	WNDCLASS wclass = { CS_HREDRAW | CS_VREDRAW, WinProc, 0, 0, Instance, 0, LoadCursor(0, IDC_ARROW), 0, 0, ItemName };
	RegisterClass(&wclass);

	auto Overlay = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST, ItemName, ItemName, WS_POPUP | WS_VISIBLE, 0, 0, X, Y, 0, 0, 0, 0);
	SetLayeredWindowAttributes(Overlay, 0, 255, LWA_ALPHA);
	DwmExtendFrameIntoClientArea(Overlay, &Margin);

	while (GetMessage(&Message, Overlay, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return 0;
}
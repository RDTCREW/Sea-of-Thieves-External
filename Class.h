#include <d2d1.h>
#include <dwrite.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dwmapi")

using namespace D2D1;
using namespace std;

auto GameFile = "SoTGame.exe";
auto GameName = "Sea of Thieves";
auto ItemName = "External Esp W/Players [SimplyHackin]";

DWORD_PTR GNames, UEngine, UViewport, UPlayer;
HANDLE Process;
RECT Frame;

ID2D1Factory* Factory;
ID2D1HwndRenderTarget* RenderTarget;
ID2D1SolidColorBrush* Brush;
ID2D1SolidColorBrush * BlackBrush;
IDWriteTextFormat* TextFormat;
IDWriteFactory* DWriteFactory;

auto Player = ColorF(ColorF::Red, 1);
auto Ammo = ColorF(ColorF::Red, 1);
auto Rowboat = ColorF(ColorF::DarkOrange, 1);
auto Ship = ColorF(ColorF::Lime, 1);
auto Cannonball = ColorF(ColorF::DarkOrange, 1);
auto Mermaid = ColorF(ColorF::LightSteelBlue, 1);
auto Crate = ColorF(ColorF::Silver, 1);
auto Ghostship = ColorF(ColorF::IndianRed, 1);
auto Enemy = ColorF(ColorF::Crimson, 1);
auto Shark = ColorF(ColorF::Firebrick, 1);
auto Shipwreck = ColorF(ColorF::White, 1);
auto Common = ColorF(ColorF::Gainsboro, 1);
auto Rare = ColorF(ColorF::DodgerBlue, 1);
auto Legendary = ColorF(ColorF::DarkOrchid, 1);
auto Mythical = ColorF(ColorF::DarkOrange, 1);
auto Cloud = ColorF(ColorF::DeepSkyBlue, 5);
auto FogStorm = ColorF(ColorF::White, 1);
auto Gunpowder = ColorF(ColorF::Yellow, 1);
auto Megalodon = ColorF(ColorF::Orange, 1);
auto Kraken = ColorF(ColorF::Firebrick, 1);
auto Outpost = ColorF(ColorF::RoyalBlue, 1);
auto Emerald = ColorF(ColorF::Green ,1);
auto Sapphire = ColorF(ColorF::MediumBlue ,1);

vector<DWORD> GNamesPattern = { 0x48, 0x89, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xC3 };
vector<DWORD> UEnginePattern = { 0x48, 0x8B, 0x35, 0x00, 0x00, 0x00, 0x00, 0x33, 0xDB };

DWORD_PTR FindPattern(DWORD_PTR Base, DWORD Size, vector<DWORD> Signature) {
	DWORD_PTR Module = Base + Size;
	size_t BytesRead;

	while (Base < Module) {
		BYTE Buffer[1024 * 64];
		ReadProcessMemory(Process, (LPVOID)Base, &Buffer, sizeof(Buffer), &BytesRead);

		if (!BytesRead)
			return 0;

		for (int i = 0; i < BytesRead; i++) {
			if (Buffer[i] != Signature[0])
				continue;

			bool Match = true;
			for (int j = 0; j < Signature.size(); j++) {
				if (!Signature[j])
					continue;

				if (Buffer[i + j] != Signature[j]) {
					Match = false;
					break;
				}
			}

			if (Match)
				return Base + i;
		}

		Base += BytesRead;
	}

	return 0;
}

template <class T, class U>
T Read(U Address) {
	T Buffer;
	ReadProcessMemory(Process, (LPVOID)Address, &Buffer, sizeof(Buffer), 0);
	return Buffer;
}

int Setup() {
	DWORD_PTR Base = 0, Address;
	DWORD PID = 0, Size = 0, Offset;

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);
	HANDLE pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	while (Process32Next(pSnapshot, &ProcEntry)) {
		if (!lstrcmpi(ProcEntry.szExeFile, GameFile)) {
			PID = ProcEntry.th32ProcessID;
			break;
		}
	}

	CloseHandle(pSnapshot);

	if (!PID)
		return 1;

	MODULEENTRY32 ModEntry;
	ModEntry.dwSize = sizeof(ModEntry);
	HANDLE mSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	while (Module32Next(mSnapshot, &ModEntry)) {
		if (!lstrcmpi(ModEntry.szModule, GameFile)) {
			Base = (DWORD_PTR)ModEntry.modBaseAddr;
			Size = ModEntry.modBaseSize;
			break;
		}
	}
//Source made by simplyHacking
	CloseHandle(mSnapshot);
	Process = OpenProcess(MAXIMUM_ALLOWED, 0, PID);

	Address = FindPattern(Base, Size, GNamesPattern);
	Offset = Read<DWORD>(Address + 0x3);
	GNames = Read<DWORD_PTR>(Address + Offset + 0x7);

	Address = FindPattern(Base, Size, UEnginePattern);
	Offset = Read<DWORD>(Address + 0x3);
	UEngine = Read<DWORD_PTR>(Address + Offset + 0x7);

	return 0;
}

struct TArray {
	DWORD_PTR Data;
	int Count;
	int Max;
};

struct FNameEntry {
	int Index;
	FNameEntry* HashNext;
	char AnsiName[1024];
};

struct FString {
	wchar_t WideName[1024];
};

struct FVector {
	float X;
	float Y;
	float Z;

	FVector operator-(FVector V) {
		return FVector{ X - V.X, Y - V.Y, Z - V.Z };
	}

	float Dot(FVector V) {
		return X * V.X + Y * V.Y + Z * V.Z;
	}
};

struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};

struct FQuat {
	float X;
	float Y;
	float Z;
	float W;
};

struct FTransform {
	FQuat Rotation;
	FVector Translation;
	FVector Scale3D;
};

struct FMinimalViewInfo {
	FVector Location;
	FRotator Rotation;
	char Unknown[0x10];
	float FOV;
};

FVector WorldToScreen(FVector WorldLocation, FMinimalViewInfo Camera, float SizeX, float SizeY) {
	auto PI = 3.14159265359f;
	auto CP = cos(Camera.Rotation.Pitch * PI / 180.f);
	auto SP = sin(Camera.Rotation.Pitch * PI / 180.f);
	auto CR = cos(Camera.Rotation.Roll * PI / 180.f);
	auto SR = sin(Camera.Rotation.Roll * PI / 180.f);
	auto CY = cos(Camera.Rotation.Yaw * PI / 180.f);
	auto SY = sin(Camera.Rotation.Yaw * PI / 180.f);

	auto X = FVector{ CP * CY, CP * SY, SP };
	auto Y = FVector{ SR * SP * CY - CR * SY, SR * SP * SY + CR * CY, -SR * CP };
	auto Z = FVector{ -(CR * SP * CY + SR * SY), CY * SR - CR * SP * SY, CR * CP };

	auto Transform = FVector{ WorldLocation.Dot(Y), WorldLocation.Dot(Z), WorldLocation.Dot(X) };
	if (Transform.Z < 0.7)
		Transform.Z = 0.7;

	FVector Location;
	Location.X = SizeX + Transform.X * (SizeX / tan(Camera.FOV * PI / 360.f)) / Transform.Z;
	Location.Y = SizeY - Transform.Y * (SizeX / tan(Camera.FOV * PI / 360.f)) / Transform.Z;

	return Location;
}
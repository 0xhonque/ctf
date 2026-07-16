import socket
import threading
import random
import sys

FLAG = "ARA7{MOV_is_turing_complete!_x86_is_redundantly_redundant,_wouldnt_you_agree?}"

KEYS = {
    "QuantumLeapster": "ARACTF_ABBD-AACA-7685-0D8F-5A7C-8E77-FD23-FEEA_7.0",
    "StarryVoyager": "ARACTF_A178-FC29-CF12-0859-B1CA-EF85-7F1A-F005_7.0",
    "MysticEchoes": "ARACTF_25C2-2D74-9C28-829A-A6EB-9F7F-DDAD-3014_7.0",
    "CosmicWhisperer": "ARACTF_A385-131E-E692-4078-9122-C76F-D9EE-2C4D_7.0",
    "PixelPioneer": "ARACTF_324A-0C6D-6208-9284-5D6C-AC67-E6C8-37FB_7.0",
    "NebulaNavigator": "ARACTF_680D-9C87-4801-2EF3-4C12-1E1B-DCCD-BF71_7.0",
    "DreamCatcherX": "ARACTF_268B-7297-70D9-E907-3EE0-913E-9382-57EE_7.0",
    "SilentSpecter": "ARACTF_E4F7-6CC2-9080-FEC0-694C-D0B4-A7C4-E92A_7.0",
    "EchoingEnigma": "ARACTF_54DD-386E-A545-2038-47DA-3EE1-D436-11ED_7.0",
    "CelestialCrafter": "ARACTF_5E66-F382-6281-82B4-F04C-E55D-80FE-5D9E_7.0",
    "GalacticGlimpse": "ARACTF_C9F4-ACD4-510F-BBFC-FE05-2E9C-FA8C-5336_7.0",
    "LunarLuminary": "ARACTF_8682-64FB-ED4E-6790-08CB-1C30-5C50-081F_7.0",
    "AstroArtisan": "ARACTF_3223-3DBD-4C51-8FDA-FF18-B9E1-8ED9-2661_7.0",
    "WhisperingWaves": "ARACTF_4561-3726-44B0-02CB-93CF-B0DD-1F96-55A0_7.0",
    "StellarScribe": "ARACTF_F19D-21A2-A646-FD59-C25D-49C4-A655-D294_7.0",
    "EnchantedEclipse": "ARACTF_85C5-5CA8-1606-F56F-EB4F-3BC8-8D63-9C6C_7.0",
    "HorizonHopper": "ARACTF_2560-7586-E60A-EBF1-114D-24DF-5077-5BC8_7.0",
    "PhantomPulse": "ARACTF_88E6-45CE-5B63-4DCC-ADD4-14D5-9D92-A86D_7.0",
    "VortexVoyage": "ARACTF_C198-BEEA-60B4-8554-A1D8-27F3-7228-7A40_7.0",
    "TwilightTrekker": "ARACTF_EA87-9BDF-72C2-1050-185E-CF9D-E1D5-F661_7.0",
    "EtherealExplorer": "ARACTF_4321-89B4-200B-8BB1-4FC7-3943-8A87-1491_7.0",
    "RadiantReverie": "ARACTF_1BA1-0296-D1A8-AAAB-D6DC-B5C6-C910-F74E_7.0",
    "NebulaNomad": "ARACTF_DE07-1392-9C96-C156-D8B3-E899-58BB-FA19_7.0",
    "CosmicCraze": "ARACTF_CB10-4CB1-5D63-7C01-0C83-FB7F-2B4A-65A8_7.0",
    "StarGazerX": "ARACTF_CA1E-93FB-C41E-BBDA-0517-71B3-2767-6BD7_7.0",
    "AuroraAdventurer": "ARACTF_5C96-DAE2-074B-8EF1-9549-18EA-21FA-64A1_7.0",
    "InfiniteIllusion": "ARACTF_0009-EFFD-7AA0-DA82-0308-808C-B396-CD0F_7.0",
    "SpectralScribe": "ARACTF_1ADF-07F3-B8A1-4CC3-A8D6-31D8-84E1-656B_7.0",
    "CelestialSurfer": "ARACTF_11C1-BFF3-8FA2-BE41-2072-6DE0-089B-5171_7.0",
    "MysticMarauder": "ARACTF_BA16-3C25-84B7-CAF8-E273-E005-D7C4-9214_7.0",
    "QuantumQuestor": "ARACTF_997F-2FA0-AD6E-0878-A322-CD5C-B640-DA9A_7.0",
    "DreamDiver": "ARACTF_51F1-1F0B-9764-6D97-A6D6-4CBB-2039-D12B_7.0",
    "SilentScribe": "ARACTF_9C03-2CBB-54DF-9A8B-87EC-110A-18CF-D544_7.0",
    "EchoingEclipse": "ARACTF_46FA-7750-6634-A208-47F4-D741-FDE5-2E17_7.0",
    "StellarStrider": "ARACTF_DE0B-F37F-DBCA-2274-CD03-3FA9-56CF-E01F_7.0",
    "EnigmaEcho": "ARACTF_68CE-27B1-AF98-EB09-EE79-E671-B796-0E9E_7.0",
    "HorizonHerald": "ARACTF_3143-CD4B-1F60-F891-FEBD-A652-0C79-5A1A_7.0",
    "PhantomFable": "ARACTF_508D-2783-024F-78E4-00AB-6F35-FE0F-39AE_7.0",
    "VortexVoyager": "ARACTF_9F13-87BC-634C-60F6-185A-098F-89A1-E9FD_7.0",
    "TwilightTrailblazer": "ARACTF_CE6F-7880-79FF-F982-5E86-4321-D82F-9411_7.0",
    "EtherealEpic": "ARACTF_7E6A-1E8D-ECAE-F172-CDED-811D-18CC-4028_7.0",
    "RadiantRover": "ARACTF_52E0-8B41-9EAE-660E-2AB4-A874-9288-4857_7.0",
    "NebulaNinja": "ARACTF_A148-6890-FC09-BF70-4051-DA3F-F956-5FFC_7.0",
    "CosmicCaptain": "ARACTF_A103-3301-4549-F1AD-B578-B0BD-C398-21D4_7.0",
    "StarryStrider": "ARACTF_739A-2641-6DE0-5325-E851-64A7-8861-F5C4_7.0",
    "AuroraArcher": "ARACTF_C04F-40F6-DE81-8CCF-E040-CA50-DA1D-E38B_7.0",
    "InfiniteInsight": "ARACTF_AFDE-7FB5-7D63-05D6-4403-62D0-B6E3-02CD_7.0",
    "SpectralSailor": "ARACTF_CC39-0E1A-3DA8-9D3B-6926-8D15-FAE6-E501_7.0",
    "CelestialCyclone": "ARACTF_BA02-5AC9-A63D-7E69-8481-DFA3-7656-2355_7.0",
    "MysticMessenger": "ARACTF_6CF2-DCB0-5E67-D149-5280-1544-72C9-2237_7.0"
}

def send(s):
    sys.stdout.write(s)
    sys.stdout.flush()

def recv():
    return sys.stdin.readline().strip()

def main():
    items = list(KEYS.items())
    random.shuffle(items)

    for idx, (username, correct_key) in enumerate(items, start=1):
        send(f"#{idx}\n")
        send(f"{username}\n")
        send("enter key: ")

        user_key = recv()

        if user_key != correct_key:
            send("Incorrect key.\n")
            return

        send("Correct!\n\n")

    send(f"Congrats!\nFlag: {FLAG}\n")

if __name__ == "__main__":
    main()

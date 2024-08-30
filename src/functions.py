import customtkinter
from PIL import Image
from src.settings import *

# Criando zona segura na tela do app, conforme no GTA.
# GET_SAFE_ZONE_SIZE = 0x3BDC44 (em decimal = 3894012)
# safezoneSizeX = app_width * 0.6 / 2
# safezoneSizeY = app_height * 0.9 / 2
# Defina a zona segura: Assim, a zona segura vai de (64, 36) até (1216, 684).
def GetSafeZoneSizeHalved():
    safeZoneSizeX = (app_width * 0.9) / 2
    safeZoneSizeY = app_height * 0.9
    return safeZoneSizeX, safeZoneSizeY

def SetMenuPositon():
    safeZoneSizeX, safeZoneSizeY = GetSafeZoneSizeHalved()
    if leftSided == 1:
        MenuXCoord1 = int(safeZoneSizeX * 0.5061)
        MenuXCoord2 = int(safeZoneSizeX * 0.7211)
        MenuXCoord3 = int(safeZoneSizeX * 0.6136)
        # Converting
        appMenuXCoord1 = MenuXCoord1 - safeZoneSizeX
        appMenuXCoord2 = MenuXCoord2 - safeZoneSizeX
        appMenuXCoord3 = MenuXCoord3 - safeZoneSizeX
        return appMenuXCoord1, appMenuXCoord2, appMenuXCoord3, leftSided
    else:
        MenuXCoord1 = int(safeZoneSizeX * 0.2789)
        MenuXCoord2 = int(safeZoneSizeX * 0.4939)
        MenuXCoord3 = int(safeZoneSizeX * 0.3864)
        # Converting
        appMenuXCoord1 = MenuXCoord1 + safeZoneSizeX
        appMenuXCoord2 = MenuXCoord2 + safeZoneSizeX
        appMenuXCoord3 = MenuXCoord3 + safeZoneSizeX
        return appMenuXCoord1, appMenuXCoord2, appMenuXCoord3, leftSided

# Resource for drawing images.
def DrawImage(path, width, height):
    set_image = customtkinter.CTkImage(
        dark_image = Image.open(path),
        size = (width, height)
    )
    return set_image

# Resource for drawing rects.
class DrawRect(customtkinter.CTkFrame):
    def __init__(self, master, width, height, fg_color):
        width = int(app_width * width)
        height = int(app_height * height)
        super().__init__(master, width = width, height = height, fg_color = fg_color, corner_radius = 0)
        # if MenuBGImage == '1':
            # self.image_path = bg_custom_img_path
            # self.GetBGImage = DrawImage(self.image_path, width = width, height = height)
            # self.DrawBGImage = customtkinter.CTkLabel(self, image = self.GetBGImage, text = "")
            # self.DrawBGImage.grid(row = 0, column = 0)
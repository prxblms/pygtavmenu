from src.functions import *

class SafeZoneFrame(customtkinter.CTkFrame):
    def __int__(self, master):
        width = 100000
        height = 100
        super().__init__(master, width = width, height = height, fg_color = '#292929', corner_radius = 1)

    # def DrawBackground(self):
    #     backgroundColor = '#292929'

    #     DrawBackground = DrawRect(self, width = 0.2250, height = 0.035, fg_color = backgroundColor)
    #     DrawBackground.place(x = self.appMenuXCoord3, y = 0)

class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()
        self.title("GTA V Ps3 Menu Designer")
        self.geometry(app_window)
        self.resizable(False, False)
        
        self.safeZoneSizeX, self.safeZoneSizeY = GetSafeZoneSizeHalved()
        self.appMenuXCoord1, self.appMenuXCoord2, self.appMenuXCoord3, self.leftSided = SetMenuPositon()
        
        # App background Image.     
        self.app_draw_bg_image()

        self.SafeZoneFrame = SafeZoneFrame(self)
        self.SafeZoneFrame.place(x = 10, y = 10)

    def app_draw_bg_image(self):
        get_bg_image = DrawImage(bg_img_path, app_width, app_height)
        app_bg_image = customtkinter.CTkLabel(self, image = get_bg_image, text = "")
        app_bg_image.grid(row = 0, column = 0)

if __name__ == '__main__':
    app = App()
    app.mainloop()
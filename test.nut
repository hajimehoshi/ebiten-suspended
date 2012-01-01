class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        if (!this.texture) {
            this.texture = ebiten.Texture("test.png")
        }
        //::print(this.texture.isCreated() + " ")
        //::print(this.texture.getWidth() + " " + this.texture.getHeight() + "\n")
        if (this.offscreen != null) {
            ::print(this.offscreen.getWidth() + "\n");
        }
    }
    function draw(offscreen) {
        //::print("draw!\n");
        this.offscreen = offscreen;
        ::print(offscreen.getWidth() + "\n");
    }
    offscreen = null;
    texture = null
    width = 0
    height = 0
}

return Game(320, 240)

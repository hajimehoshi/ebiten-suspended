class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        if (!this.texture) {
            // TODO: resource path
            //this.texture = ebiten.Texture("test.png")
        }
        //::print(this.texture.isCreated() + " ")
        //::print(this.texture.getWidth() + " " + this.texture.getHeight() + "\n")
        if (this.offscreen) {
            ::print(this.offscreen.getWidth() + "\n")
        }
    }
    function draw(offscreen) {
        //::print("draw!\n");
        //::print(offscreen.getWidth() + "\n");
        offscreen.drawRect(10, 10, 100, 100, 255, 0, 0, 255)
        this.offscreen = offscreen
        ::print(this.offscreen.getWidth() + "\n")
    }
    offscreen = null
    texture = null
    width = 0
    height = 0
}

return Game(320, 240)

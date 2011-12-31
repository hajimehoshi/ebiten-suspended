class Game {
    constructor(width, heigth) {
        this.texture = null
        this.width = width
        this.height = height
    }
    function update() {
        /*if (!this.texture) {
            this.texture = ebiten.Texture("hoge")
        }*/
    }
    function draw() {
        //::print("draw!\n");
    }
    texture = null
    width = 0
    height = 0
}

return Game(320, 240)

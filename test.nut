class Game {
    constructor(width, heigth) {
        this.width = width
        this.height = height
    }
    function update() {
        ::print("update!\n");
    }
    function draw() {
        ::print("draw!\n");
    }
    width = 0
    height = 0
}

return Game(320, 240)

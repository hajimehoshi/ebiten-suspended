class Game {
    constructor(width, heigth) {
        this.width = width
        this.height = height
    }
    function update(tf) {
        //tf.load();
        //tf.create();
        ::print(tf + "\n");
        ::print(typeof(tf) + "\n");
    }
    function draw() {
        //::print("draw!\n");
    }
    width = 0
    height = 0
}

local hoge = ebiten.GeometryMatrix()
local fuga = ebiten.GeometryMatrix()
local piyo = ebiten.GeometryMatrix()

::print(hoge + "\n")

return Game(320, 240)

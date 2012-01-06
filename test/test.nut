function assert(exp) {
    local stackinfo = ::getstackinfos(2)
    if (!exp) {
        ::print("\nline " + stackinfo.line + " failed!")
        throw "test failed"
    }
    ::print(".")
}

class Foo {
    static function _get(str) {
        ::print(str + "\n")
    }
    static identity = null
}
::print(Foo.identity + "\n")
Foo.identity <- 3

class Test {
    function update() {
        if (updateTestExecuted && drawTestExecuted) {
            ::print("\n")
            ::print("Squirrel Test End\n")
            ebiten.terminate()
        }
        if (updateTestExecuted) {
            return
        }
        {
            local geo = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.7)
            ::assert(geo instanceof ebiten.GeometryMatrix)
            ::assert(geo.a == 3.1)
            ::assert(geo.b == 4.1)
            ::assert(geo.c == 5.9)
            ::assert(geo.d == 2.6)
            ::assert(geo.tx == 5.3)
            ::assert(geo.ty == 5.7)
        }
        {
            local geo = ebiten.GeometryMatrix.identity
            ::assert(geo instanceof ebiten.GeometryMatrix)
            ::assert(geo.a == 1)
            ::assert(geo.b == 0)
            ::assert(geo.c == 0)
            ::assert(geo.d == 1)
            ::assert(geo.tx == 0)
            ::assert(geo.ty == 0)
        }
        updateTestExecuted = true
    }
    function draw(offscreen) {
        if (drawTestExecuted) {
            return
        }
        ::assert(offscreen instanceof ebiten.Texture)
        drawTestExecuted = true
    }
    updateTestExecuted = false
    drawTestExecuted = false
}

::print("Squirrel Test Start\n")

return Test()

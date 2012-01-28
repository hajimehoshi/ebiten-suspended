class Foo {}

class AssertFailure {
    constructor(message) {
        this.message = message
    }
    function _tostring() {
        return this.message
    }
    message = null
}

function assert(exp) {
    local stackinfo = ::getstackinfos(2)
    if (!exp) {
        ::print("line " + stackinfo.line + " failed!")
        throw AssertFailure("test failed\n")
    }
}

function test_update_system(system) {
    ::assert(system instanceof ebiten.System);
    local input = system.input
    ::assert(input instanceof ebiten.Input);
    ::assert(typeof(input.touchX) == "integer");
    ::assert(typeof(input.touchY) == "integer");
    ::assert(typeof(input.isTouched) == "bool");
}

function test_update_GeometryMatrix_constructor(system) {
    local g = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.8)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 4.1)
    ::assert(g.c == 5.9)
    ::assert(g.d == 2.6)
    ::assert(g.tx == 5.3)
    ::assert(g.ty == 5.8)
}

function test_update_GeometryMatrix_identity(system) {
    local g = ebiten.GeometryMatrix.identity
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_concat(system) {
    local a = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.8)
    local b = ebiten.GeometryMatrix(9.7, 9.3, 2.3, 8.4, 6.2, 6.4)
    {
        local c = a.concat(b)
        ::assert(::abs(a.a  - 3.1)    < 0.0001)
        ::assert(::abs(a.b  - 4.1)    < 0.0001)
        ::assert(::abs(a.c  - 5.9)    < 0.0001)
        ::assert(::abs(a.d  - 2.6)    < 0.0001)
        ::assert(::abs(a.tx - 5.3)    < 0.0001)
        ::assert(::abs(a.ty - 5.8)    < 0.0001)
        ::assert(::abs(b.a  - 9.7)    < 0.0001)
        ::assert(::abs(b.b  - 9.3)    < 0.0001)
        ::assert(::abs(b.c  - 2.3)    < 0.0001)
        ::assert(::abs(b.d  - 8.4)    < 0.0001)
        ::assert(::abs(b.tx - 6.2)    < 0.0001)
        ::assert(::abs(b.ty - 6.4)    < 0.0001)
        ::assert(::abs(c.a  - 84.94)  < 0.0001)
        ::assert(::abs(c.b  - 63.95)  < 0.0001)
        ::assert(::abs(c.c  - 56.69)  < 0.0001)
        ::assert(::abs(c.d  - 31.27)  < 0.0001)
        ::assert(::abs(c.tx - 111.55) < 0.0001)
        ::assert(::abs(c.ty - 67.31)  < 0.0001)
    }
    {
        local c = b.concat(a)
        ::assert(::abs(a.a  - 3.1)   < 0.0001)
        ::assert(::abs(a.b  - 4.1)   < 0.0001)
        ::assert(::abs(a.c  - 5.9)   < 0.0001)
        ::assert(::abs(a.d  - 2.6)   < 0.0001)
        ::assert(::abs(a.tx - 5.3)   < 0.0001)
        ::assert(::abs(a.ty - 5.8)   < 0.0001)
        ::assert(::abs(b.a  - 9.7)   < 0.0001)
        ::assert(::abs(b.b  - 9.3)   < 0.0001)
        ::assert(::abs(b.c  - 2.3)   < 0.0001)
        ::assert(::abs(b.d  - 8.4)   < 0.0001)
        ::assert(::abs(b.tx - 6.2)   < 0.0001)
        ::assert(::abs(b.ty - 6.4)   < 0.0001)
        ::assert(::abs(c.a  - 39.5)  < 0.0001)
        ::assert(::abs(c.b  - 63.27) < 0.0001)
        ::assert(::abs(c.c  - 63.21) < 0.0001)
        ::assert(::abs(c.d  - 76.71) < 0.0001)
        ::assert(::abs(c.tx - 50.76) < 0.0001)
        ::assert(::abs(c.ty - 59.02) < 0.0001)
    }
    {
        try {
            c = a.concat(Foo())
            ::assert(false)
        } catch (e) {
            if (e instanceof AssertFailure) {
                throw e
            }
        }
    }
}

function test_update_GeometryMatrix_scale(system) {
    local g = ebiten.GeometryMatrix.scale(3.1, 4.1)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 4.1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_scaleWithCenter(system) {
}

function test_update_GeometryMatrix_rotate(system) {
    local theta = 0
    {
        local g = ebiten.GeometryMatrix.rotate(theta)
        ::assert(g instanceof ebiten.GeometryMatrix)
        ::assert(::abs(g.a - ::cos(theta))  < 0.0001)
        ::assert(::abs(g.b - -::sin(theta)) < 0.0001)
        ::assert(::abs(g.c - ::sin(theta))  < 0.0001)
        ::assert(::abs(g.d - ::cos(theta))  < 0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
    local theta = 1
    {
        local g = ebiten.GeometryMatrix.rotate(theta)
        ::assert(g instanceof ebiten.GeometryMatrix)
        ::assert(::abs(g.a - ::cos(theta))  < 0.0001)
        ::assert(::abs(g.b - -::sin(theta)) < 0.0001)
        ::assert(::abs(g.c - ::sin(theta))  < 0.0001)
        ::assert(::abs(g.d - ::cos(theta))  < 0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
}

function test_update_GeometryMatrix_rotateWithCenter(system) {
}

function test_update_ColorMatrix_constructor(system) {
    {
        local values = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
        ]
        local c = ebiten.ColorMatrix(values)
        ::assert(c instanceof ebiten.ColorMatrix)
        /*::assert(c.value(0, 0) == 3.1)
        ::assert(c.value(0, 1) == 4.1)
        ::assert(c.value(0, 4) == 5.3)*/
    }
}

/*function test_update_ColorMatrix_identity(system) {
    local c = ebiten.ColorMatrix.identity
    ::assert(c instanceof ebiten.ColorMatrix)
    ::assert(c.e00 == 1)
}*/

function test_update_ColorMatrix_concat(system) {
}

function test_update_Texture_constructor(system) {
    local t = ebiten.Texture(10, 20)
    ::assert(t.width == 10)
    ::assert(t.height == 20)
}

function test_draw_offscrreen(offscreen) {
    ::assert(offscreen instanceof ebiten.Texture)
}

class Test {
    function update(system) {
        if (updateTestExecuted && drawTestExecuted) {
            ::print("Squirrel Test End\n")
            ebiten.terminate()
        }
        if (updateTestExecuted) {
            return
        }
        foreach (member, value in ::getroottable()) {
            if (::type(value) != "function") {
                continue;
            }
            if (!::regexp("^test_update_").search(member)) {
                continue;
            }
            ::print("  " + member + "\n")
            value(system)
        }
        // TODO: Another texture test 
        updateTestExecuted = true
    }
    function draw(offscreen) {
        if (drawTestExecuted) {
            return
        }
        foreach (member, value in ::getroottable()) {
            if (::type(value) != "function") {
                continue;
            }
            if (!::regexp("^test_draw_").search(member)) {
                continue;
            }
            ::print("  " + member + "\n")
            value(offscreen)
        }
        drawTestExecuted = true
    }
    updateTestExecuted = false
    drawTestExecuted = false
}

::print("Squirrel Test Start\n")

return Test()

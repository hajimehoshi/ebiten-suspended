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
    ::assertLine(exp, stackinfo.line)
}

function assertDelta(a, b, delta) {
    local stackinfo = ::getstackinfos(2)
    ::assertLine(::abs(a - b) < delta, stackinfo.line)
}

function assertError(func) {
    local stackinfo = ::getstackinfos(2)
    try {
        func()
        ::assertLine(false, stackinfo.line)
    } catch (e) {
        if (e instanceof AssertFailure) {
            throw e
        }
    }
}

function assertLine(exp, line) {
    if (exp) {
        return
    }
    ::print("line " + line + " failed!")
    throw AssertFailure("test failed\n")
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
        ::assertDelta(a.a,  3.1,  0.0001)
        ::assertDelta(a.b,  4.1,    0.0001)
        ::assertDelta(a.c,  5.9,    0.0001)
        ::assertDelta(a.d,  2.6,    0.0001)
        ::assertDelta(a.tx, 5.3,    0.0001)
        ::assertDelta(a.ty, 5.8,    0.0001)
        ::assertDelta(b.a,  9.7,    0.0001)
        ::assertDelta(b.b,  9.3,    0.0001)
        ::assertDelta(b.c,  2.3,    0.0001)
        ::assertDelta(b.d,  8.4,    0.0001)
        ::assertDelta(b.tx, 6.2,    0.0001)
        ::assertDelta(b.ty, 6.4,    0.0001)
        ::assertDelta(c.a,  84.94,  0.0001)
        ::assertDelta(c.b,  63.95,  0.0001)
        ::assertDelta(c.c,  56.69,  0.0001)
        ::assertDelta(c.d,  31.27,  0.0001)
        ::assertDelta(c.tx, 111.55, 0.0001)
        ::assertDelta(c.ty, 67.31,  0.0001)
    }
    {
        local c = b.concat(a)
        ::assertDelta(a.a,  3.1,   0.0001)
        ::assertDelta(a.b,  4.1,   0.0001)
        ::assertDelta(a.c,  5.9,   0.0001)
        ::assertDelta(a.d,  2.6,   0.0001)
        ::assertDelta(a.tx, 5.3,   0.0001)
        ::assertDelta(a.ty, 5.8,   0.0001)
        ::assertDelta(b.a,  9.7,   0.0001)
        ::assertDelta(b.b,  9.3,   0.0001)
        ::assertDelta(b.c,  2.3,   0.0001)
        ::assertDelta(b.d,  8.4,   0.0001)
        ::assertDelta(b.tx, 6.2,   0.0001)
        ::assertDelta(b.ty, 6.4,   0.0001)
        ::assertDelta(c.a,  39.5,  0.0001)
        ::assertDelta(c.b,  63.27, 0.0001)
        ::assertDelta(c.c,  63.21, 0.0001)
        ::assertDelta(c.d,  76.71, 0.0001)
        ::assertDelta(c.tx, 50.76, 0.0001)
        ::assertDelta(c.ty, 59.02, 0.0001)
    }
    {
        ::assertError(function () {
                a.concat(Foo())                
            })
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
        ::assertDelta(g.a, ::cos(theta),  0.0001)
        ::assertDelta(g.b, -::sin(theta), 0.0001)
        ::assertDelta(g.c, ::sin(theta),  0.0001)
        ::assertDelta(g.d, ::cos(theta),  0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
    local theta = 1
    {
        local g = ebiten.GeometryMatrix.rotate(theta)
        ::assert(g instanceof ebiten.GeometryMatrix)
        ::assertDelta(g.a, ::cos(theta),  0.0001)
        ::assertDelta(g.b, -::sin(theta), 0.0001)
        ::assertDelta(g.c, ::sin(theta),  0.0001)
        ::assertDelta(g.d, ::cos(theta),  0.0001)
        ::assert(g.tx == 0)
        ::assert(g.ty == 0)
    }
}

function test_update_GeometryMatrix_rotateWithCenter(system) {
}

function test_update_ColorMatrix_constructor(system) {
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
        ]
        local c = ebiten.ColorMatrix(elements)
        ::assert(c instanceof ebiten.ColorMatrix)
        ::assert(c.e00 == 3.1)
        ::assert(c.e01 == 4.1)
        ::assert(c.e02 == 5.9)
        ::assert(c.e03 == 2.6)
        ::assert(c.e04 == 5.3)
        ::assert(c.e10 == 5.8)
        ::assert(c.e11 == 9.7)
        ::assert(c.e12 == 9.3)
        ::assert(c.e13 == 2.3)
        ::assert(c.e14 == 8.4)
        ::assert(c.e20 == 6.2)
        ::assert(c.e21 == 6.4)
        ::assert(c.e22 == 3.3)
        ::assert(c.e23 == 8.3)
        ::assert(c.e24 == 2.7)
        ::assert(c.e30 == 9.5)
        ::assert(c.e31 == 0.2)
        ::assert(c.e32 == 8.8)
        ::assert(c.e33 == 4.1)
        ::assert(c.e34 == 9.7)
        ::assert(c.e40 == 0)
        ::assert(c.e41 == 0)
        ::assert(c.e42 == 0)
        ::assert(c.e43 == 0)
        ::assert(c.e44 == 1)
        ::assertError(function () {
                local v = c.e54
            })
        ::assertError(function () {
                local v = c.e45
            })
        ::assertError(function () {
                local v = c.eaa
            })
    }
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1,
        ]
        ::assertError(function () {
                local c = ebiten.ColorMatrix(elements)
            })
    }
    {
        local elements = [
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
            1.6,
        ]
        ::assertError(function () {
                local c = ebiten.ColorMatrix(elements)
            })
    }
}

function test_update_ColorMatrix_identity(system) {
    local c = ebiten.ColorMatrix.identity
    ::assert(c instanceof ebiten.ColorMatrix)
    ::assert(c.e00 == 1)
    ::assert(c.e01 == 0)
    ::assert(c.e02 == 0)
    ::assert(c.e03 == 0)
    ::assert(c.e04 == 0)
    ::assert(c.e10 == 0)
    ::assert(c.e11 == 1)
    ::assert(c.e12 == 0)
    ::assert(c.e13 == 0)
    ::assert(c.e14 == 0)
    ::assert(c.e20 == 0)
    ::assert(c.e21 == 0)
    ::assert(c.e22 == 1)
    ::assert(c.e23 == 0)
    ::assert(c.e24 == 0)
    ::assert(c.e30 == 0)
    ::assert(c.e31 == 0)
    ::assert(c.e32 == 0)
    ::assert(c.e33 == 1)
    ::assert(c.e34 == 0)
    ::assert(c.e40 == 0)
    ::assert(c.e41 == 0)
    ::assert(c.e42 == 0)
    ::assert(c.e43 == 0)
    ::assert(c.e44 == 1)
}

function test_update_ColorMatrix_concat(system) {
    local c1 = ebiten.ColorMatrix([
            3.1, 4.1, 5.9, 2.6, 5.3,
            5.8, 9.7, 9.3, 2.3, 8.4,
            6.2, 6.4, 3.3, 8.3, 2.7,
            9.5, 0.2, 8.8, 4.1, 9.7,
        ])
    local c2 = ebiten.ColorMatrix([
            1.6, 9.3, 9.9, 3.8, 1.9,
            6.8, 2.1, 4.8, 0.8, 6.5,
            1.3, 2.8, 2.3, 0.6, 6.4,
            7.0, 9.3, 8.4, 4.6, 0.9,
        ])
    {
        local c3 = c1.concat(c2)
        ::assertDelta(c3.e00, 156.38, 0.0001)
        ::assertDelta(c3.e01, 160.89, 0.0001)
        ::assertDelta(c3.e02, 162.04, 0.0001)
        ::assertDelta(c3.e03, 123.30, 0.0001)
        ::assertDelta(c3.e04, 152.09, 0.0001)
        ::assertDelta(c3.e10, 70.62,  0.0001)
        ::assertDelta(c3.e11, 79.13,  0.0001)
        ::assertDelta(c3.e12, 82.53,  0.0001)
        ::assertDelta(c3.e13, 65.63,  0.0001)
        ::assertDelta(c3.e14, 80.90,  0.0001)
        ::assertDelta(c3.e20, 40.23,  0.0001)
        ::assertDelta(c3.e21, 47.33,  0.0001)
        ::assertDelta(c3.e22, 46.58,  0.0001)
        ::assertDelta(c3.e23, 31.37,  0.0001)
        ::assertDelta(c3.e24, 48.84,  0.0001)
        ::assertDelta(c3.e30, 171.42, 0.0001)
        ::assertDelta(c3.e31, 173.59, 0.0001)
        ::assertDelta(c3.e32, 195.99, 0.0001)
        ::assertDelta(c3.e33, 128.17, 0.0001)
        ::assertDelta(c3.e34, 183.42, 0.0001)
        ::assert(c3.e40 == 0)
        ::assert(c3.e41 == 0)
        ::assert(c3.e42 == 0)
        ::assert(c3.e43 == 0)
        ::assert(c3.e44 == 1)
    }
    {
        local c3 = c2.concat(c1)
        ::assertDelta(c3.e00, 58.71,  0.0001)
        ::assertDelta(c3.e01, 78.14,  0.0001)
        ::assertDelta(c3.e02, 85.78,  0.0001)
        ::assertDelta(c3.e03, 30.56,  0.0001)
        ::assertDelta(c3.e04, 77.94,  0.0001)
        ::assertDelta(c3.e10, 103.43, 0.0001)
        ::assertDelta(c3.e11, 121.74, 0.0001)
        ::assertDelta(c3.e12, 144.69, 0.0001)
        ::assertDelta(c3.e13, 45.96,  0.0001)
        ::assertDelta(c3.e14, 144.06, 0.0001)
        ::assertDelta(c3.e20, 115.83, 0.0001)
        ::assertDelta(c3.e21, 157.53, 0.0001)
        ::assertDelta(c3.e22, 169.41, 0.0001)
        ::assertDelta(c3.e23, 68.84,  0.0001)
        ::assertDelta(c3.e24, 84.67,  0.0001)
        ::assertDelta(c3.e30, 56.70,  0.0001)
        ::assertDelta(c3.e31, 151.54, 0.0001)
        ::assertDelta(c3.e32, 149.69, 0.0001)
        ::assertDelta(c3.e33, 60.40,  0.0001)
        ::assertDelta(c3.e34, 89.06,  0.0001)
        ::assert(c3.e40 == 0)
        ::assert(c3.e41 == 0)
        ::assert(c3.e42 == 0)
        ::assert(c3.e43 == 0)
        ::assert(c3.e44 == 1)
    }
    ::assertError(function () {
            c1.concat(Foo())                
        })
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

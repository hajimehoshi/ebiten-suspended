function assert(exp) {
    local stackinfo = ::getstackinfos(2)
    if (!exp) {
        ::print("line " + stackinfo.line + " failed!")
        throw "test failed"
    }
}

function test_update_GeometryMatrix_constructor() {
    local g = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.7)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 4.1)
    ::assert(g.c == 5.9)
    ::assert(g.d == 2.6)
    ::assert(g.tx == 5.3)
    ::assert(g.ty == 5.7)
}

function test_update_GeometryMatrix_identity() {
    local g = ebiten.GeometryMatrix.identity
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_concat() {
}

function test_update_GeometryMatrix_scale() {
    local g = ebiten.GeometryMatrix.scale(3.1, 4.1)
    ::assert(g instanceof ebiten.GeometryMatrix)
    ::assert(g.a == 3.1)
    ::assert(g.b == 0)
    ::assert(g.c == 0)
    ::assert(g.d == 4.1)
    ::assert(g.tx == 0)
    ::assert(g.ty == 0)
}

function test_update_GeometryMatrix_scaleWithCenter() {
}

function test_update_GeometryMatrix_rotate() {
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

function test_update_GeometryMatrix_rotateWithCenter() {
}

function test_update_Texture_constructor() {
    local t = ebiten.Texture(10, 20)
    ::assert(t.isCreated == false)
    try {
        t.width
        ::assert(false)
    } catch (e) {
    }
    try {
        t.height
        ::assert(false)
    } catch (e) {
    }
}

function test_draw_offscrreen(offscreen) {
    ::assert(offscreen instanceof ebiten.Texture)
    ::assert(offscreen.isCreated)
}

class Test {
    function update() {
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
            value()
        }
        // TODO: Color matrix test
        // TODO: Another texture test 
        // TODO: Sprite test
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

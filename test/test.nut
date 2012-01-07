function assert(exp) {
    local stackinfo = ::getstackinfos(2)
    if (!exp) {
        ::print("line " + stackinfo.line + " failed!")
        throw "test failed"
    }
}

function testGeometryMatrixConstructor() {
    local geo = ebiten.GeometryMatrix(3.1, 4.1, 5.9, 2.6, 5.3, 5.7)
    ::assert(geo instanceof ebiten.GeometryMatrix)
    ::assert(geo.a == 3.1)
    ::assert(geo.b == 4.1)
    ::assert(geo.c == 5.9)
    ::assert(geo.d == 2.6)
    ::assert(geo.tx == 5.3)
    ::assert(geo.ty == 5.7)
}

function testGeometryMatrixIdentity() {
    local geo = ebiten.GeometryMatrix.identity
    ::assert(geo instanceof ebiten.GeometryMatrix)
    ::assert(geo.a == 1)
    ::assert(geo.b == 0)
    ::assert(geo.c == 0)
    ::assert(geo.d == 1)
    ::assert(geo.tx == 0)
    ::assert(geo.ty == 0)
}

function testGeometryMatrixConcat() {
}

function testGeometryMatrixScale() {
}

function testGeometryMatrixScaleWithCenter() {
}

function testGeometryMatrixRotate() {
}

function testGeometryMatrixRotateWithCenter() {
}

function testTextureConstructor() {
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
            if (!::regexp("^test").search(member)) {
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
        ::assert(offscreen instanceof ebiten.Texture)
        drawTestExecuted = true
    }
    updateTestExecuted = false
    drawTestExecuted = false
}

::print("Squirrel Test Start\n")

return Test()

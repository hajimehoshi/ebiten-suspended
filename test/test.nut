function assert(exp) {
    local stackinfo = ::getstackinfos(2)
    ::print("line " + stackinfo.line + ": ")
    if (!exp) {
        ::print("Failed!\n")
        throw "test failed"
    }
    ::print("OK\n")
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
        ::assert(true)
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

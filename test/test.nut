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
    ::doAssert(exp, stackinfo.src, stackinfo.line)
}

function assertDelta(a, b, delta) {
    local stackinfo = ::getstackinfos(2)
    ::doAssert(::abs(a - b) < delta, stackinfo.src, stackinfo.line)
}

function assertError(func) {
    local stackinfo = ::getstackinfos(2)
    try {
        func()
        ::doAssert(false, stackinfo.src, stackinfo.line)
    } catch (e) {
        if (e instanceof AssertFailure) {
            throw e
        }
    }
}

function doAssert(exp, file, line) {
    if (exp) {
        return
    }
    ::print(file + "[" + line + "] failed!\n")
    throw AssertFailure("test failed\n")
}

function test_draw_offscrreen(offscreen) {
    ::assert(offscreen instanceof ebiten.Texture)
}

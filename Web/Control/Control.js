document.addEventListener("DOMContentLoaded", function () {
    const joystick1 = document.getElementById("joystick1");
    const joystick2 = document.getElementById("joystick2");

    let joystick1X = 0;
    let joystick1Y = 0;
    let joystick2X = 0;
    let joystick2Y = 0;

    joystick1.style.left = "20px";
    joystick1.style.bottom = "20px";

    joystick2.style.right = "20px";
    joystick2.style.bottom = "20px";

    joystick1.addEventListener("mousedown", startDrag1);
    joystick2.addEventListener("mousedown", startDrag2);

    function startDrag1(e) {
        e.preventDefault();
        document.addEventListener("mousemove", drag1);
        document.addEventListener("mouseup", stopDrag1);
    }

    function startDrag2(e) {
        e.preventDefault();
        document.addEventListener("mousemove", drag2);
        document.addEventListener("mouseup", stopDrag2);
    }

    function drag1(e) {
        joystick1X = e.clientX - 20;
        joystick1Y = window.innerHeight - e.clientY - 20;

        updateJoystickPosition(joystick1, joystick1X, joystick1Y);
    }

    function drag2(e) {
        joystick2X = window.innerWidth - e.clientX - 20;
        joystick2Y = window.innerHeight - e.clientY - 20;

        updateJoystickPosition(joystick2, joystick2X, joystick2Y);
    }

    function stopDrag1() {
        document.removeEventListener("mousemove", drag1);
        document.removeEventListener("mouseup", stopDrag1);
    }

    function stopDrag2() {
        document.removeEventListener("mousemove", drag2);
        document.removeEventListener("mouseup", stopDrag2);
    }

    function updateJoystickPosition(joystick, x, y) {
        joystick.style.left = x + "px";
        joystick.style.bottom = y + "px";
    }
});

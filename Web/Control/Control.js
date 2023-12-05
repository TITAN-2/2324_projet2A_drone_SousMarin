var joystickHandle = document.getElementById('joystick-vitesse-handle');
var container = document.getElementById('joystick-vitesse-container');
var isDragging = false;

joystickHandle.addEventListener('mousedown', function (e) {
    isDragging = true;
    container.style.transition = 'none';
    updateJoystick(e);
});

document.addEventListener('mousemove', function (e) {
    if (isDragging) {
        updateJoystick(e);
    }
});

document.addEventListener('mouseup', function () {
    if (isDragging) {
        isDragging = false;
        container.style.transition = 'all 0.3s ease';
        resetJoystick();
    }
});

function updateJoystick(e) {
    var containerRect = container.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height / 2;

    var distance = containerRect.height / 2; // Diviser par 2 pour rester dans la plage de -distance à distance
    var offsetY = Math.min(Math.max(y, -1.80*distance), -distance/4.8); // Limiter la position verticale

    joystickHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';
}


function updateJoystick1(e) {
    var containerRect = container.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height/2;

    var distance = containerRect.height / 2;
    var offsetY = Math.min(Math.max(y, -distance), distance/1.68); // Limiter la position verticale

    joystickHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';
}

function resetJoystick() {
    joystickHandle.style.transform = 'translate(-50%, -50%)';
}

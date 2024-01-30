var joystickVitesseHandle = document.getElementById('joystick-vitesse-handle');
var joystickVitesseContainer = document.getElementById('joystick-vitesse-container');
var isDraggingVitesse = false;

var joystickProfondeurHandle = document.getElementById('joystick-profondeur-handle');
var joystickProfondeurContainer = document.getElementById('joystick-profondeur-container');
var isDraggingProfondeur = false;

var joystickDirectionHandle = document.getElementById('joystick-direction-handle');
var joystickDirectionContainer = document.getElementById('joystick-direction-container');
var isDraggingDirection = false;

resetVitesseValue();
resetProfondeurValue();
resetDirectionValue();

// Event listeners for Joystick Vitesse
joystickVitesseHandle.addEventListener('mousedown', function (e) {
    isDraggingVitesse = true;
    joystickVitesseContainer.style.transition = 'none';
    updateJoystickVitesse(e, joystickVitesseHandle, joystickVitesseContainer, isDraggingVitesse);
});

// Event listeners for Joystick Profondeur
joystickProfondeurHandle.addEventListener('mousedown', function (e) {
    isDraggingProfondeur = true;
    joystickProfondeurContainer.style.transition = 'none';
    updateJoystickProfondeur(e, joystickProfondeurHandle, joystickProfondeurContainer, isDraggingProfondeur);
});

// Event listeners for Joystick Direction
joystickDirectionHandle.addEventListener('mousedown', function (e) {
    isDraggingDirection = true;
    joystickDirectionContainer.style.transition = 'none';
    updateJoystickDirection(e, joystickDirectionHandle, joystickDirectionContainer, isDraggingDirection);
});

document.addEventListener('mousemove', function (e) {
    if (isDraggingVitesse) {
        updateJoystickVitesse(e, joystickVitesseHandle, joystickVitesseContainer, isDraggingVitesse);
    }
    if (isDraggingProfondeur) {
        updateJoystickProfondeur(e, joystickProfondeurHandle, joystickProfondeurContainer, isDraggingProfondeur);
    }
    if (isDraggingDirection) {
        updateJoystickDirection(e, joystickDirectionHandle, joystickDirectionContainer, isDraggingDirection);
    }
});

document.addEventListener('mouseup', function () {
    if (isDraggingVitesse) {
        isDraggingVitesse = false;
        joystickVitesseContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickVitesseHandle);
        resetVitesseValue();
    }
    if (isDraggingProfondeur) {
        isDraggingProfondeur = false;
        joystickProfondeurContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickProfondeurHandle);
        resetProfondeurValue();
    }
    if (isDraggingDirection) {
        isDraggingDirection = false;
        joystickDirectionContainer.style.transition = 'all 0.3s ease';
        resetJoystick(joystickDirectionHandle);
        resetDirectionValue();
    }
});

function updateJoystickVitesse(e) {
    var containerRect = joystickVitesseContainer.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height;

    var distance = containerRect.height /2;
    var offsetY = Math.min(Math.max(y, -1.80 * distance), -distance / 4.8);

    joystickVitesseHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';

    var vitesseValue = calculateJoystickValueVitesse(joystickVitesseContainer, joystickVitesseHandle);
    document.getElementById('vitesse-value').innerText = vitesseValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function updateJoystickProfondeur(e) {
    var containerRect = joystickProfondeurContainer.getBoundingClientRect();
    var y = e.clientY - containerRect.top - containerRect.height / 2  -25;

    var distance = containerRect.height / 2;
    var offsetY = Math.min(Math.max(y, -distance), distance / 1.68);

    joystickProfondeurHandle.style.transform = 'translate(-50%, ' + offsetY + 'px)';

    var profondeurValue = calculateJoystickValueProfondeur(joystickProfondeurContainer, joystickProfondeurHandle);
    document.getElementById('profondeur-value').innerText = profondeurValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function updateJoystickDirection(e) {
    var containerRect = joystickDirectionContainer.getBoundingClientRect();
    var x = e.clientX - containerRect.left - containerRect.width /2  - 25;

    var distance = containerRect.width / 2;
    var offsetX = Math.min(Math.max(x, -distance), distance/1.3515);

    joystickDirectionHandle.style.transform = 'translate(' + offsetX + 'px, -50%)';

    var directionValue = calculateJoystickValueDirection(joystickDirectionContainer, joystickDirectionHandle);
    document.getElementById('direction-value').innerText = directionValue.toFixed(1); // Ajustez le formatage selon vos besoins
}

function resetJoystick(handle) {
    handle.style.transform = 'translate(-50%, -50%)';
}



function calculateJoystickValueProfondeur(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;

    value = (handleRect.top - containerRect.top) / containerRect.height;
    value = (0.8-value)*100/0.8 -50;
    return value;
}

function calculateJoystickValueVitesse(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;

    value = (handleRect.top - containerRect.top) / containerRect.height;
    value = (0.8-value)*100/0.8;
    return value;
}

function calculateJoystickValueDirection(container, handle) {
    // Calculer la valeur relative de la position de la poignée dans le conteneur
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();

    var value;
    value = (handleRect.left - containerRect.left) / containerRect.width;
    value = value*100/0.87 -50;
    return value;
}


function resetVitesseValue() {
    document.getElementById('vitesse-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}

function resetProfondeurValue() {
    document.getElementById('profondeur-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}

function resetDirectionValue() {
    document.getElementById('direction-value').innerText = '0.0'; // Mettez la valeur par défaut selon vos besoins
}



// Définition des joysticks avec leurs propriétés spécifiques
var joysticks = [
    {  handle: document.getElementById('joystick-vitesse-handle'), container: document.getElementById('joystick-vitesse-container'), isDragging: false,
        offsetY: 0, // Offset vertical spécifique pour ce joystick
        orientation: 'vertical', // Orientation spécifique pour ce joystick
        valueCommand : 0
    },
    { handle: document.getElementById('joystick-profondeur-handle'), container: document.getElementById('joystick-profondeur-container'), isDragging: false,
        offsetY: 50, // Offset vertical spécifique pour ce joystick
        orientation: 'vertical', // Orientation spécifique pour ce joystick
        valueCommand : 0
    },
    { handle: document.getElementById('joystick-direction-handle'), container: document.getElementById('joystick-direction-container'), isDragging: false,
        offsetX: 50, // Offset horizontal spécifique pour ce joystick
        orientation: 'horizontal', // Orientation spécifique pour ce joystick
        valueCommand : 0
    }
];

// Réinitialisation des valeurs des joysticks
resetJoysticks();

// Ajout des gestionnaires d'événements pour chaque joystick
joysticks.forEach(function(joystick) {
    joystick.handle.addEventListener('mousedown', function (e) {
        joystick.isDragging = true;
        joystick.container.style.transition = 'none';
        updateJoystick(e, joystick);
    });
});

document.addEventListener('mousemove', function (e) {
    joysticks.forEach(function(joystick) {
        if (joystick.isDragging) {
            updateJoystick(e, joystick);
        }
    });
});

document.addEventListener('mouseup', function () {
    joysticks.forEach(function(joystick) {
        if (joystick.isDragging) {
            joystick.isDragging = false;
            joystick.container.style.transition = 'all 0.3s ease';
            //resetJoystick(joystick);
            //resetValue(joystick);
            sendUart();
        }
    });
});

function updateJoystick(e, joystick) {
    var containerRect = joystick.container.getBoundingClientRect();
    var offsetX = 0;
    var offsetY = 0;

    if (joystick.orientation === 'vertical') {
        offsetY = e.clientY - containerRect.top - containerRect.height / 2 + joystick.offsetY;
    } else if (joystick.orientation === 'horizontal') {
        offsetX = e.clientX - containerRect.left - containerRect.width / 2 + joystick.offsetX;
    }

    joystick.handle.style.transform = 'translate(' + offsetX + 'px, ' + offsetY + 'px)';

    var value = calculateJoystickValue(joystick.container, joystick.handle);
    document.getElementById(joystick.container.id.replace('-container', '-value')).innerText = value.toFixed(1);
}

function resetJoystick(joystick) {
    joystick.handle.style.transform = 'translate(-50%, -50%)';
}

function resetValue(joystick) {
    document.getElementById(joystick.container.id.replace('-container', '-value')).innerText = '0.0';
}

function resetJoysticks() {
    joysticks.forEach(function(joystick) {
        resetJoystick(joystick);
        resetValue(joystick);
    });
}

function calculateJoystickValue(container, handle) {
    var containerRect = container.getBoundingClientRect();
    var handleRect = handle.getBoundingClientRect();
    var value;

    if (containerRect.height > containerRect.width) {
        // Vertical joystick
        value = (handleRect.top - containerRect.top) / containerRect.height;
        value = (0.8 - value) * 100 / 0.8;
    } else {
        // Horizontal joystick
        value = (handleRect.left - containerRect.left) / containerRect.width;
        value = value * 100 / 0.87 ;
    }

    return value;
}

function sendUart() {
    $.ajax({
        url: '/update_values',  // L'URL à laquelle envoyer la requête
        method: 'POST',
        data: {
            vitesse: joysticks[0].valueCommand,
            profondeur: joysticks[1].valueCommand,
            direction: joysticks[2].valueCommand
        },
        success: function(response) {
            // Gestion de la réponse du serveur (si nécessaire)
            console.log(response);
        },
        error: function(error) {
            // Gestion des erreurs (si nécessaire)
            console.error(error);
        }
    });
}
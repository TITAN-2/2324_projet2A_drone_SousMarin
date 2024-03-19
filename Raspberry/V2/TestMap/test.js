// Définition des joysticks avec leurs propriétés spécifiques
var joysticks = [
    {  handle: document.getElementById('joystick-vitesse-handle'), container: document.getElementById('joystick-vitesse-container'), isDragging: false,
        offsetY: 0, // Offset vertical spécifique pour ce joystick
        orientation: 'vertical', // Orientation spécifique pour ce joystick
        valueCommand : 0
    },
    { handle: document.getElementById('joystick-profondeur-handle'), container: document.getElementById('joystick-profondeur-container'), isDragging: false,
        offsetY: 0, // Offset vertical spécifique pour ce joystick
        orientation: 'vertical', // Orientation spécifique pour ce joystick
        valueCommand : 0
    },
    { handle: document.getElementById('joystick-direction-handle'), container: document.getElementById('joystick-direction-container'), isDragging: false,
        offsetX: 0, // Offset horizontal spécifique pour ce joystick
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
    // Obtenir les dimensions du conteneur du joystick
    var containerRect = joystick.container.getBoundingClientRect();
    
    // Calculer les nouvelles coordonnées du centre du joystick en fonction de la position de la souris
    var handleWidth = joystick.handle.offsetWidth;
    var handleHeight = joystick.handle.offsetHeight;
    var centerX = e.clientX - containerRect.left;
    var centerY = e.clientY - containerRect.top
    
    // Limiter les déplacements du joystick pour rester à l'intérieur du conteneur
    centerX = Math.max(handleWidth/2, Math.min(containerRect.width - handleWidth/2, centerX));
    centerY = Math.max(handleHeight/2, Math.min(containerRect.height - handleHeight/2, centerY));
    // Mettre à jour la position du joystick
    if (joystick.orientation === "vertical")
        joystick.handle.style.top = centerY + 'px';
    if (joystick.orientation === "horizontal")
        joystick.handle.style.left = centerX + 'px';
    // Calculer et afficher la valeur du joystick (à adapter en fonction de vos besoins)
    var value = calculateJoystickValue(joystick);
    joystick.valueCommand = value
    document.getElementById(joystick.container.id.replace('-container', '-value')).innerText = value.toFixed(1);}
    
    
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

function calculateJoystickValue(joystick) {
    var containerRect = joystick.container.getBoundingClientRect();
    var joystickHandle = joystick.handle.getBoundingClientRect();
    var value;

    if (containerRect.height > containerRect.width) {
        // Vertical joystick
        var joystickPos =containerRect.bottom- joystickHandle.bottom;
        var range = containerRect.height - joystickHandle.height;
        value = (joystickPos / range) * 100;
    } else {
        // Horizontal joystick
        var joystickPos = joystickHandle.left - containerRect.left;
        var range = containerRect.width - joystickHandle.width;
        value = (joystickPos / range) * 100;
    }
    // On limite la valeur a 0-100
    value = Math.min(Math.max(value, 0), 100);
    
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





// Fonction pour générer une position aléatoire
function generatePosition() {
    return {
        x: Math.random() * 295, // Coordonnée x aléatoire (250 pour rester dans les limites du conteneur)
        y: Math.random() * 295  // Coordonnée y aléatoire (250 pour rester dans les limites du conteneur)
    };
}

// Variable pour stocker la position
var position = generatePosition();

// Fonction pour afficher la position et la tracer sur la carte
function updatePosition() {
    position = generatePosition();
    console.log("Nouvelle position:", position);
    drawMarker(position.x, position.y);
}

// Fonction pour tracer un marqueur sur la carte
function drawMarker(x, y) {
    var map = document.getElementById('map');
    var marker = document.createElement('div');
    marker.style.width = '5px';
    marker.style.height = '5px';
    marker.style.backgroundColor = 'red';
    marker.style.position = 'absolute';
    marker.style.left = x + 'px';
    marker.style.top = y + 'px';
    map.appendChild(marker);
}

// Appel de la fonction updatePosition toutes les 3 secondes
setInterval(updatePosition, 2);
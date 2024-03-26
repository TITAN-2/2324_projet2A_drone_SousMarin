// RPICom.js

$(document).ready(function() {
    // Ajoute un gestionnaire d'événement pour le changement de valeur des boutons
    $('#joystick-vitesse-handle, #joystick-profondeur-handle, #joystick-direction-handle').on('change', function() {
        // Obtient les nouvelles valeurs des boutons
        var vitesseValue = $('#vitesse-value').val();
        var profondeurValue = $('#profondeur-value').val();
        var directionValue = $('#direction-value').val();

        // Met à jour les valeurs affichées
        $('#vitesse-value').text(vitesseValue);
        $('#profondeur-value').text(profondeurValue);
        $('#direction-value').text(directionValue);

        // Vous pouvez ajouter ici la logique d'envoi des valeurs via UART
        // en utilisant RPICom.js ou tout autre moyen nécessaire.
        // Par exemple, envoi d'une requête AJAX au serveur Flask.

        // Exemple de requête AJAX vers le serveur Flask (assurez-vous d'avoir Flask-SocketIO)
        $.ajax({
            url: '/update_values',  // L'URL à laquelle envoyer la requête
            method: 'POST',
            data: {
                vitesse: vitesseValue,
                profondeur: profondeurValue,
                direction: directionValue
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
    });
});

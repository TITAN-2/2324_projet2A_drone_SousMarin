document.addEventListener('DOMContentLoaded', function () {
    // Sélectionne l'élément input de type range
    var sliders = document.querySelectorAll('.Trust #slider, .Depth #slider, .Angle #slider');
    
    sliders.forEach(function (slider) {
        var sectionName = slider.closest('section').className;
        var dashBoard = document.getElementById('DashBoard');
        var div = dashBoard.querySelector('.' + sectionName);
        var level = div.querySelector('#level');
        
        // Met à jour la valeur affichée lorsque la valeur du slider change
        slider.addEventListener('input', function () {
            var value = slider.value;
            level.textContent = value;
        });
    });
});

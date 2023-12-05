document.addEventListener('DOMContentLoaded', function() {
    const joystickContainer = document.getElementById('joystick-container');
    const joystickHandle = document.getElementById('joystick-handle');
    
    const joystickVertContainer = document.getElementById('joystick_vert-container');
    const joystickVertHandle = document.getElementById('joystick_vert-handle');

    let isDragging = false;

    joystickHandle.addEventListener('mousedown', startDrag);
    document.addEventListener('mouseup', endDrag);
    document.addEventListener('mousemove', drag);

    joystickVertHandle.addEventListener('mousedown', startVertDrag);
    document.addEventListener('mouseup', endVertDrag);
    document.addEventListener('mousemove', vertDrag);

    function startDrag(e) {
        isDragging = true;
        joystickHandle.setPointerCapture(e.pointerId);
    }

    function endDrag() {
        isDragging = false;
        resetJoystickPosition();
    }

    function drag(e) {
        if (isDragging) {
            const containerRect = joystickContainer.getBoundingClientRect();
            const x = e.clientX - containerRect.left - containerRect.width / 2;
            const y = e.clientY - containerRect.top - containerRect.height / 2;
            const distance = Math.sqrt(x*x + y*y);

            if (distance <= containerRect.width / 2) {
                joystickHandle.style.transition = 'none';
                joystickHandle.style.transform = `translate(-50%, -50%) translate(${x}px, ${y}px)`;
            } else {
                const angle = Math.atan2(y, x);
                const maxX = Math.cos(angle) * (containerRect.width / 2);
                const maxY = Math.sin(angle) * (containerRect.height / 2);

                joystickHandle.style.transition = 'none';
                joystickHandle.style.transform = `translate(-50%, -50%) translate(${maxX}px, ${maxY}px)`;
            }
        }
    }

    function resetJoystickPosition() {
        joystickHandle.style.transition = 'transform 0.2s ease-out';
        joystickHandle.style.transform = 'translate(-50%, -50%)';
    }

    // Joystick vertical functions

    function startVertDrag(e) {
        isDragging = true;
        joystickVertHandle.setPointerCapture(e.pointerId);
    }

    function endVertDrag() {
        isDragging = false;
        resetVertJoystickPosition();
    }

    function vertDrag(e) {
        if (isDragging) {
            const vertContainerRect = joystickVertContainer.getBoundingClientRect();
            const vertY = e.clientY - vertContainerRect.top - vertContainerRect.height / 2;
            const vertDistance = Math.abs(vertY);

            if (vertDistance <= vertContainerRect.height / 2) {
                joystickVertHandle.style.transition = 'none';
                joystickVertHandle.style.transform = `translate(-50%, -50%) translate(0px, ${vertY}px)`;
            } else {
                const vertMax = Math.sign(vertY) * (vertContainerRect.height / 2);

                joystickVertHandle.style.transition = 'none';
                joystickVertHandle.style.transform = `translate(-50%, -50%) translate(0px, ${vertMax}px)`;
            }
        }
    }

    function resetVertJoystickPosition() {
        joystickVertHandle.style.transition = 'transform 0.2s ease-out';
        joystickVertHandle.style.transform = 'translate(-50%, -50%)';
    }
});

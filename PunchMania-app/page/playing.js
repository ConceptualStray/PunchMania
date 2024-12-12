setInterval(() => {
    fetch('/control/status.php')
        .then(response => response.text())
        .then(data => {
            if (data.trim() === 'false') {
                window.location.href = '/';
            }
        })
        .catch(error => console.error('Error:', error));
}, 2000);

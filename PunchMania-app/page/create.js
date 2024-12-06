let startTime = 0;
let lastPressTime = 0;
let isPlaying = false;
let isPaused = false;
let pauseStartTime = 0;
let timestamps = [];
let testTimeouts = [];

window.addEventListener('load', function() {
    var songId = document.getElementById('songId').value;
    var audioPlayer = document.getElementById('audioPlayer');
    var songPath = `/data/${songId}/song.mp3`;
    
    audioPlayer.src = songPath;
    audioPlayer.load();
});

document.getElementById('playButton').addEventListener('click', function() {
    var audioPlayer = document.getElementById('audioPlayer');
    var offset = parseInt(document.getElementById('offsetSlider').value) || 0;
    
    audioPlayer.currentTime = offset / 1000; // Convert ms to seconds
    audioPlayer.play();

    // Remove focus from the play button
    this.blur();

    audioPlayer.addEventListener('timeupdate', updateRemainingTime);
    audioPlayer.addEventListener('timeupdate', updateCurrentTime);
});

document.getElementById('pauseButton').addEventListener('click', function() {
    var audioPlayer = document.getElementById('audioPlayer');
    if (isPaused) {
        audioPlayer.play();
        isPaused = false;
        this.textContent = 'Pause';
    } else {
        pauseStartTime = Date.now();
        audioPlayer.pause();
        isPaused = true;
        this.textContent = 'Unpause';
    }

    // Remove focus from the pause button
    this.blur();
});

document.getElementById('audioPlayer').addEventListener('play', function() {
    // var offset = parseInt(document.getElementById('offsetSlider').value) || 0;
	var audioPlayer = document.getElementById('audioPlayer');
    // startTime = Date.now() - offset;
    lastPressTime = Math.floor(audioPlayer.currentTime * 1000);
    isPlaying = true;
});

document.getElementById('audioPlayer').addEventListener('loadedmetadata', function() {
    document.getElementById('offsetSlider').max = this.duration * 1000; // Convert seconds to ms
});

document.getElementById('stopButton').addEventListener('click', function() {
    var audioPlayer = document.getElementById('audioPlayer');
    audioPlayer.pause();
    audioPlayer.currentTime = 0;
    isPlaying = false;
    isPaused = false;
    document.getElementById('pauseButton').textContent = 'Pause';

    // Remove focus from the stop button
    this.blur();

    audioPlayer.removeEventListener('timeupdate', updateRemainingTime);
    audioPlayer.removeEventListener('timeupdate', updateCurrentTime);
    document.getElementById('remainingTime').textContent = 'Remaining time: 0s';
    document.getElementById('currentTime').textContent = 'Current time: 0 ms';
});

document.getElementById('clearButton').addEventListener('click', function() {
    let logContainer = document.getElementById('logContainer');
    logContainer.innerHTML = '';
    timestamps = [];

    // Remove focus from the clear button
    this.blur();
});

document.getElementById('selectAllButton').addEventListener('click', function() {
    let logContainer = document.getElementById('logContainer');
    let range = document.createRange();
    range.selectNodeContents(logContainer);
    let selection = window.getSelection();
    selection.removeAllRanges();
    selection.addRange(range);

    // Remove focus from the select all button
    this.blur();
});

document.getElementById('volumeSlider').addEventListener('input', function() {
    var audioPlayer = document.getElementById('audioPlayer');
    audioPlayer.volume = this.value / 100;
});

document.getElementById('offsetSlider').addEventListener('input', function() {
    let offsetMs = this.value;
    let offsetMinutes = Math.floor(offsetMs / 60000);
    let offsetRemainingSeconds = ((offsetMs % 60000) / 1000).toFixed(2);
    document.getElementById('offsetValue').textContent = `Current offset: ${offsetMs} ms (${offsetMinutes} min ${offsetRemainingSeconds} s)`;
});

document.getElementById('testButton').addEventListener('click', function() {
    var audioPlayer = document.getElementById('audioPlayer');
    var offset = parseInt(document.getElementById('offsetSlider').value) || 0;

    // Stop the test if it is already running
    if (testTimeouts.length > 0) {
        testTimeouts.forEach(timeout => clearTimeout(timeout));
        testTimeouts = [];
        audioPlayer.pause();
        audioPlayer.currentTime = 0;
        return;
    }

    audioPlayer.currentTime = offset / 1000; // Convert ms to seconds
    audioPlayer.play();

    let blinkCircle = document.getElementById('blinkCircle');
    timestamps.forEach((timestamp, index) => {
        let timeout = setTimeout(() => {
            blinkCircle.style.visibility = 'visible';
            setTimeout(() => {
                blinkCircle.style.visibility = 'hidden';
            }, 100);
        }, timestamp - offset);
        testTimeouts.push(timeout);
    });

    // Remove focus from the test button
    this.blur();

    // Stop the test when there are no more timestamps
    let stopTimeout = setTimeout(() => {
        audioPlayer.pause();
        audioPlayer.currentTime = 0;
        testTimeouts = [];
    }, Math.max(...timestamps) - offset);
    testTimeouts.push(stopTimeout);
});

document.addEventListener('keydown', function(event) {
    if (event.code === 'Space' && isPlaying && !isPaused) {
        let audioPlayer = document.getElementById('audioPlayer');
        let currentTimeMs = Math.floor(audioPlayer.currentTime * 1000); // Convert seconds to ms
        let timeSinceLastPress = currentTimeMs - lastPressTime;

        let logContainer = document.getElementById('logContainer');
        let logEntry = document.createElement('div');
        logEntry.textContent = `${currentTimeMs}:${timeSinceLastPress}`;
        logContainer.appendChild(logEntry);

        // Scroll to the bottom to show the latest entry
        logContainer.scrollTop = logContainer.scrollHeight;

        lastPressTime = currentTimeMs;
        timestamps.push(currentTimeMs);
    }
});

function updateRemainingTime() {
    var audioPlayer = document.getElementById('audioPlayer');
    var remainingTime = audioPlayer.duration - audioPlayer.currentTime;
    document.getElementById('remainingTime').textContent = `Remaining time: ${Math.ceil(remainingTime)}s`;
}

function updateCurrentTime() {
    var audioPlayer = document.getElementById('audioPlayer');
    var currentTimeMs = Math.floor(audioPlayer.currentTime * 1000); // Convert seconds to ms
    document.getElementById('currentTime').textContent = `Current time: ${currentTimeMs} ms`;
}
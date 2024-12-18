let audio = null;

document.addEventListener('DOMContentLoaded', function() {
	var elems = document.querySelectorAll('.carousel');
	M.Carousel.init(elems, {
		onCycleTo:activeTileCallback,
		padding: -350
	});
});

let timeoutId;
function activeTileCallback(el) {
	if (timeoutId) {
		clearTimeout(timeoutId);
	}
	$('#progressBar').removeClass('hide');
	$('#content').addClass('hide');
	if (audio) {
		audio.pause();
		audio.currentTime = 0;
		audio = null;
	}
	timeoutId = setTimeout(() => {
		var id = $(el).attr('songId');
		$.get('/control/controls.php?id=' + id, function(data){
			$('#content').html(data);
			$('#content').removeClass('hide');
			$('#progressBar').addClass('hide');
		});
	}, 700);
}

$(document).on('click', '#songPreview', function() {
	const songId = $('#songId').val();
	const songUrl = `/data/${songId}/song.mp3`;
	const startTime = parseFloat($('#startTime').val()) || 0;
	if (!audio) {
		audio = new Audio(songUrl);
		audio.volume = 0.5; // Set volume to 50%
		audio.currentTime = startTime;
		audio.play();
	} else if (audio.paused) {
		audio.currentTime = startTime;
		audio.play();
	} else {
		audio.pause();
		audio.currentTime = startTime;
	}
});

$('body').on('input', '#offset', function(){
	var offset = $(this).val();
	$('#offsetValue').html(offset);
});

$('body').on('range-changed', (e) => {
	const data = e.detail;
	$('#startTime').val(data.minRangeValue);
	$('#endTime').val(data.maxRangeValue);
	// data = { sliderId: null, minRangeValue: 0, maxRangeValue: 1000 }
});
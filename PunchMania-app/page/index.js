
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
	timeoutId = setTimeout(() => {
		var id = $(el).attr('songId');
		$.get('/control/controls.php?id=' + id, function(data){
			$('#content').html(data);
			$('#content').removeClass('hide');
			$('#progressBar').addClass('hide');
		});
	}, 700);
}


$('body').on('input','#offset', function(){
	var offset = $(this).val();
	$('#offsetValue').html(offset);
});

document.addEventListener('range-changed', (e) => {
	const data = e.detail;
	$('#startTime').val(data.minRangeValue);
	$('#endTime').val(data.maxRangeValue);
	// data = { sliderId: null, minRangeValue: 0, maxRangeValue: 1000 }
  });
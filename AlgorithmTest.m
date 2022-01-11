% gecgelcem 11.01.2021
% BOUN EE443 PROJECT E

clc();
clear();
close('all');

width = 320;
height = 240;
positions = 0:1:width;

map = generate(width, height, positions);
show(map, width, height, positions, 'Terrain Generation');

function show(map, width, height, positions, header)
	figure();
	hold('on');
	title(header);
	xlabel('X Position');
	xlim([0 width]);
	ylabel('Height Map');
	ylim([-height 0]);
	stem(positions, ceil(map), 'Marker', 'none', 'LineWidth', 4);
end

function map = generate(width, height, positions)
	angles = x(positions, width, 0.0, 2 * pi);
	peakCount = r(0.75, 1.5);
	start = r(0, 2 * pi);
	% 0.0 is Top & 1.0 is Bottom
	peakHeight = r(0.5, 0.67);
	valleyHeight = r(0.8, 0.95);
	map = s(angles * peakCount + start, peakHeight, valleyHeight) * -height;
end

function x = x(p, width, min, max)
	x = span(p, 0.0, width, min, max);
end

function s = s(x, min, max)
	s = span(sin(x), -1.0, 1.0, min, max);
end

function r = r(min, max)
	r = span(rand(), 0.0, 1.0, min, max);
end

function a = span(a, currentMin, currentMax, targetMin, targetMax)
	currentSpan = currentMax - currentMin;
	a = (a - currentMin) / currentSpan;
	targetSpan = targetMax - targetMin;
	a = a * targetSpan + targetMin;
end
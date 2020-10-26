jsonText = fileread('Portfolio.json');
% Convert JSON formatted text to MATLAB data types (3x1 cell array in this example)
jsonData = jsondecode(jsonText); 
% Change HighPrice value in Row 3 from 10000 to 12000
jsonData{3}.HighPrice = 12000;
% Convert to JSON text
jsonText2 = jsonencode(jsonData);
% Write to a json file
fid = fopen('Portfolio2.json', 'w');
fprintf(fid, '%s', jsonText2);
fclose(fid);
pkg load instrument-control;

esp = serialport("COM18", 115200);
flush(esp);

disp("Listening... Press the ESP32 Reset button.");

sweep_data = [];

while true
    try
        raw_line = readline(esp);
        clean_line = strtrim(char(raw_line));

        % PRINT THE RAW LINE to the screen so we can see what's happening
        disp(clean_line);

        if ~isempty(strfind(clean_line, "Sweep_Complete"))
            disp("Sweep finished. Closing port.");
            break;
        end

        % Ensure there are exactly 2 commas and no header text
        if sum(clean_line == ',') == 2 && isempty(strfind(clean_line, "Vce_V"))

            string_values = strsplit(clean_line, ",");
            numeric_values = str2double(string_values);

            if length(numeric_values) == 3 && ~any(isnan(numeric_values))
                sweep_data = [sweep_data; numeric_values];
            end
        end
    catch
        disp("Caught a corrupted byte - discarding line.");
        continue;
    end
end

clear esp;

% This line safely checks if the matrix actually has data before saving
if ~isempty(sweep_data)
    csvwrite('tracer_data.csv', sweep_data);
    disp("Data capture complete and saved to tracer_data.csv!");
else
    disp("ERROR: sweep_data is empty. Nothing to save.");
end

from fm_station_database import fm_station_database

# Define FM band ranges and unlicensed rules for various countries
fm_band_rules = {
    "USA": {
        "fm_band": (88.0, 108.0),  # FCC-regulated FM band
        "unlicensed_range": [88.1, 107.9],  # All valid FM frequencies for unlicensed use
        "notes": "Unlicensed transmissions are governed by FCC Part 15 regulations."
    },
    "India": {
        "fm_band": (88.0, 108.0),
        "unlicensed_range": [],
        "notes": "India does not allow unlicensed FM broadcasting."
    },
    "UK": {
        "fm_band": (87.5, 108.0),
        "unlicensed_range": [87.6, 88.0],
        "notes": "Unlicensed use is allowed for short-range devices like FM transmitters."
    },
    "Europe": {
        "fm_band": (87.5, 108.0),
        "unlicensed_range": [],
        "notes": "Unlicensed FM broadcasting is generally prohibited."
    },
}

def get_occupied_frequencies(country):
    """
    Get all occupied frequencies for a specific country.
    """
    return [round(station["frequency"], 1) for station in fm_station_database if station["country"].lower() == country.lower()]

def find_whitespace(country, step=0.2):
    """
    Find whitespace frequencies in the FM band for a specified country.
    """
    if country not in fm_band_rules:
        raise ValueError(f"No FM band information available for {country}.")
    
    # Fetch rules for the country
    rules = fm_band_rules[country]
    fm_band = rules["fm_band"]
    unlicensed_range = rules["unlicensed_range"]
    occupied_frequencies = get_occupied_frequencies(country)

    # Identify whitespace frequencies
    whitespace_frequencies = []
    current_freq = fm_band[0]

    while current_freq <= fm_band[1]:
        current_freq = round(current_freq, 1)

        # Check if the frequency is not occupied and falls in the unlicensed range (if specified)
        if current_freq not in occupied_frequencies:
            if not unlicensed_range or (current_freq >= unlicensed_range[0] and current_freq <= unlicensed_range[1]):
                whitespace_frequencies.append(current_freq)
        
        current_freq += step

    return whitespace_frequencies, rules

if __name__ == "__main__":
    print("Welcome to the FM Whitespace Scanner!")
    
    # Get user input for country
    country = input("Enter the country to scan for FM whitespace (e.g., USA, India, UK): ").strip()
    
    try:
        # Find whitespace frequencies for the given country
        whitespace, rules = find_whitespace(country)
        
        print(f"\nFM Band for {country}: {rules['fm_band'][0]} MHz to {rules['fm_band'][1]} MHz")
        print(f"Notes: {rules['notes']}")
        
        if whitespace:
            print(f"\nWhitespace frequencies available in the FM band ({country}):")
            print(", ".join([f"{freq} MHz" for freq in whitespace]))
        else:
            print(f"\nNo whitespace frequencies found in the FM band for {country}.")
    except ValueError as e:
        print(f"\nError: {e}")


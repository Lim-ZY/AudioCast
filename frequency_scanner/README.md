## Unallocated Frequency Scanner

This python utility identifies unallocated frequencies (whitespace) in the FM broadcast spectrum based on the user's location (country), using publicly available data to build a database of allocated channels. These unallocated frequencies can then be used for custom transmitters to avoid congestion and interference with existing radio stations.

We encourage and welcome open source contributions to enhance the allocated FM frequencies [database](./fm_station_database.py).

### Sample Input - Output

**Pre-requisites**:
```
Suitable version of python / python3
```

**Execution**:
```
python fm_whitespace_scanner.py
```

**Input**:
```
Enter the country to scan for FM whitespace (e.g., USA, India, UK): USA
```

**Output**:
```
FM Band for USA: 88.0 MHz to 108.0 MHz
Notes: Unlicensed transmissions are governed by FCC Part 15 regulations.

Whitespace frequencies available in the FM band (USA):
88.2 MHz, 88.4 MHz, 88.6 MHz, 88.8 MHz, 89.0 MHz, 89.2 MHz, 89.4 MHz,..., 
97.6 MHz, 97.8 MHz, 98.0 MHz, 98.2 MHz, 98.4 MHz, 98.6 MHz, 98.8 MHz, 99.0 MHz, ...,
 104.4 MHz, 104.6 MHz, 104.8 MHz, 105.0 MHz, 105.2 MHz, 105.4 MHz, ...

```

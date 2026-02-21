# Matlab Group Project
🌍 Global Development Visualization MATLAB App
Overview

This project is a MATLAB App Designer application that visualizes global development indicators using interactive maps, plots, histograms, and statistics. The app allows users to explore country-level data (such as schooling and other development metrics), display geographic heatmaps, generate graphs, and view statistical summaries.

The project combines:

An interactive GUI built in MATLAB App Designer

Excel-based global development datasets

Geographic border rendering for world maps

Modular plotting and analysis functions

The goal is to provide an intuitive way to compare development indicators across countries and regions.

✨ Features

🌐 World Map Visualization

Country borders and labels

Heatmap-style coloring based on selected indicators

📊 Graphs & Histograms

Schooling graphs

Histograms for selected metrics

📈 Statistical Analysis

Summary statistics via dedicated functions

⚙️ Preset Selections

Quickly load predefined views or metrics

🧭 Interactive App Interface

Built with MATLAB App Designer (groupProjectApp.mlapp)

🧩 Main Application
Entry Point

Open and run:

groupProjectApp.mlapp


in MATLAB App Designer.

This is the final combined version of the project.

Earlier prototypes:

ProjectApp.mlapp

ProjectUI.mlapp

📁 File Structure
App / UI

groupProjectApp.mlapp — Main application

ProjectApp.mlapp — early app version

ProjectUI.mlapp — UI prototype

Core Logic & Visualization

Project.m — main project logic

ProjDataLoad.m — loads Excel data into MATLAB

sortData.m — cleans and sorts development data

Plot.m — general plotting helper

mapFunc.m — world map visualization

histogramFunc.m — histogram generation

schoolingGraphFunc.m — schooling-specific plots

presetFunc.m — preset configurations

statisticsFunc.m — statistical calculations

statisticsFunc2.m — additional statistics

testingCode.m, testFile.m — testing / sandbox scripts

Geographic Borders System

Used for drawing and labeling countries on maps:

borders.m

bordersm.m

labelborders.m

labelbordersm.m

borderdata.mat

Documentation:

borders_documentation.m

borders_documentation.mlx

bordersm_documentation.m

bordersm_documentation.mlx

Data Files

EGProjectData.xlsx — original dataset

Fully Sorted EG Project Data.xlsx — cleaned/sorted version

Miscellaneous

.asv files — MATLAB autosaves (optional)

.DS_Store — macOS system file (can be ignored)

Image files — example outputs and legends

▶️ How to Run
Requirements

MATLAB (R2022a or newer recommended)

Mapping Toolbox (recommended for geographic plotting)

Steps

Place all project files in the same directory (or preserve relative paths).

Open MATLAB.

Set the project folder as your current directory.

Open:

groupProjectApp.mlapp


Click Run in App Designer.

The app will load the Excel dataset and initialize the visualizations.

📊 Data Notes

Data is loaded from EGProjectData.xlsx or Fully Sorted EG Project Data.xlsx.

ProjDataLoad.m handles importing.

sortData.m organizes indicators for plotting and analysis.

👥 Contributions

This project was developed collaboratively.

Nathaniel (Me)

MATLAB App Designer integration

Core plotting logic (Plot.m, map + histogram integration)

Data loading and processing pipeline

Statistics functions

UI wiring and feature integration

Henry (Partner)

Geographic border system (borders*.m, labelborders*.m)

Border dataset (borderdata.mat)

Mapping documentation

Supporting visualization utilities

Final integration was completed in groupProjectApp.mlapp.

⚠️ Known Issues / Limitations

Requires Mapping Toolbox for best results.

Some .asv files may appear — these are autosaves and not required.

Performance may slow with large datasets or repeated redraws.

Country naming must match border data exactly for labeling.

🚀 Future Improvements

Add dropdowns for more indicators

Improve performance of map rendering

Add export-to-image / CSV functionality

Add region-based aggregation

Improve UI layout and tooltips

Add time-series support

📜 License / Academic Use

Based on the Borders script by Chad A. Greene.

This project was created for academic purposes.
Reuse is permitted with attribution.

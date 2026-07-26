#pragma once

enum class ActivePanel {
    Packages,
    Drivers,
    Warehouse,
    Routes,
    Dispatch,
    Requests,
    Tracking,
    Reports,
    Undo,
    Settings
};

void renderPackagePanel();
void renderDriverPanel();
void renderWarehousePanel();
void renderGraphPanel();
void renderDispatchPanel();
void renderRequestPanel();
void renderTrackingPanel();
void renderReportsPanel();
void renderUndoPanel();
void renderSettingsPanel();

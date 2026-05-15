#ifndef CANVAS_CRASH_REPORT_H
#define CANVAS_CRASH_REPORT_H

typedef struct CanvasCrashReport {

    int preferredWidth;
    int preferredHeight;
    int minimumWidth;
    int minimumHeight;
} CanvasCrashReport;

void CanvasCrashReport_construct(CanvasCrashReport *self, int var1);

#endif

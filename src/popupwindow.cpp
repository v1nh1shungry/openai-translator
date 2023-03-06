#include "popupwindow.h"
#include "./ui_popupwindow.h"

#include <QClipboard>
#include <atomic>

static HHOOK hHook = nullptr;
enum MouseState { PRESSED, MOVING, RELEASED };
std::atomic<MouseState> mouse_state = RELEASED;

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    switch (wParam) {
    case WM_LBUTTONDOWN:
      mouse_state = PRESSED;
      break;
    case WM_LBUTTONUP:
      if (mouse_state == MOVING)
        PopupWindow::instance()->setupJob();
      mouse_state = RELEASED;
      break;
    case WM_MOUSEMOVE:
      mouse_state =
          (mouse_state == PRESSED || mouse_state == MOVING) ? MOVING : RELEASED;
      break;
    }
  }
  return CallNextHookEx(hHook, nCode, wParam, lParam);
}

PopupWindow *PopupWindow::instance() {
  static PopupWindow inst;
  return &inst;
}

PopupWindow::PopupWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::PopupWindow) {
  ui->setupUi(this);
  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint |
                 Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint |
                 Qt::WindowStaysOnTopHint);
  QGuiApplication::clipboard()->clear();
  hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, nullptr, 0);
}

PopupWindow::~PopupWindow() {
  delete ui;
  UnhookWindowsHookEx(hHook);
}

void PopupWindow::setupJob() {
  INPUT inputs[4];
  ZeroMemory(inputs, sizeof(inputs));
  inputs[0].type = INPUT_KEYBOARD;
  inputs[0].ki.wVk = VK_LCONTROL;
  inputs[1].type = INPUT_KEYBOARD;
  inputs[1].ki.wVk = 'C';
  inputs[2].type = INPUT_KEYBOARD;
  inputs[2].ki.wVk = 'C';
  inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
  inputs[2].type = INPUT_KEYBOARD;
  inputs[2].ki.wVk = VK_LCONTROL;
  inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
  emit activated(QGuiApplication::clipboard()->text());
}

void PopupWindow::display(QString text) { ui->textBrowser->setText(text); }

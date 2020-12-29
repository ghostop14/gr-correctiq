#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Dcoffset Test
# GNU Radio version: v3.8.0.0-845-g0901fcb5

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import eng_notation
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import blocks
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import correctiq
import osmosdr
import time

from gnuradio import qtgui

class dcoffset_test(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Dcoffset Test", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Dcoffset Test")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "dcoffset_test")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2.4e6
        self.real = real = -0.011900
        self.imag = imag = -0.004100
        self.gain = gain = 16
        self.center_freq = center_freq = 95.7e6
        self.amp_on = amp_on = 14

        ##################################################
        # Blocks
        ##################################################
        self._real_range = Range(-0.1, 0.1, 0.0001, -0.011900, 200)
        self._real_win = RangeWidget(self._real_range, self.set_real, 'real', "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_grid_layout.addWidget(self._real_win, 1, 0, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._imag_range = Range(-0.1, 0.1, 0.0001, -0.004100, 200)
        self._imag_win = RangeWidget(self._imag_range, self.set_imag, 'imag', "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_grid_layout.addWidget(self._imag_win, 1, 1, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._gain_range = Range(0, 40, 8, 16, 200)
        self._gain_win = RangeWidget(self._gain_range, self.set_gain, 'Gain', "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_grid_layout.addWidget(self._gain_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._center_freq_tool_bar = Qt.QToolBar(self)
        self._center_freq_tool_bar.addWidget(Qt.QLabel('Center Frequency' + ": "))
        self._center_freq_line_edit = Qt.QLineEdit(str(self.center_freq))
        self._center_freq_tool_bar.addWidget(self._center_freq_line_edit)
        self._center_freq_line_edit.returnPressed.connect(
            lambda: self.set_center_freq(eng_notation.str_to_num(str(self._center_freq_line_edit.text()))))
        self.top_grid_layout.addWidget(self._center_freq_tool_bar, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_c(
            2048, #size
            samp_rate, #samp_rate
            "", #name
            4, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-0.01, 0.01)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(True)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Add Const I', 'Add Const Q', 'Manual CorrectIQ I', 'Manual CorrectIQ Q', 'Auto CorrectIQ I',
            'Auto CorrectIQ Q', 'Full CorrectIQ I', 'Full CorrectIQ Q', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(8):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
            2048, #size
            window.WIN_BLACKMAN_hARRIS, #wintype
            center_freq, #fc
            samp_rate, #bw
            "", #name
            4,
            None # parent
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-120, -20)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(True)
        self.qtgui_freq_sink_x_0.set_fft_average(0.2)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        self.qtgui_freq_sink_x_0.set_fft_window_normalized(False)



        labels = ['Add Const', 'Manual Offset', 'AutoSync Offset', 'CorrectIQ', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(4):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.osmosdr_source_0 = osmosdr.source(
            args="numchan=" + str(1) + " " + 'rtl'
        )
        self.osmosdr_source_0.set_time_unknown_pps(osmosdr.time_spec_t())
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(center_freq, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(gain, 0)
        self.osmosdr_source_0.set_if_gain(0, 0)
        self.osmosdr_source_0.set_bb_gain(0, 0)
        self.osmosdr_source_0.set_antenna('', 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
        self.correctiq_correctiq_man_0 = correctiq.correctiq_man(real, imag)
        self.correctiq_correctiq_auto_0 = correctiq.correctiq_auto(samp_rate, center_freq, gain, 2)
        self.correctiq_correctiq_1 = correctiq.correctiq()
        self.blocks_add_const_vxx_1_0 = blocks.add_const_cc(real + 1j * imag)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_add_const_vxx_1_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.blocks_add_const_vxx_1_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.correctiq_correctiq_1, 0), (self.qtgui_freq_sink_x_0, 3))
        self.connect((self.correctiq_correctiq_1, 0), (self.qtgui_time_sink_x_0, 3))
        self.connect((self.correctiq_correctiq_auto_0, 0), (self.qtgui_freq_sink_x_0, 2))
        self.connect((self.correctiq_correctiq_auto_0, 0), (self.qtgui_time_sink_x_0, 2))
        self.connect((self.correctiq_correctiq_man_0, 0), (self.qtgui_freq_sink_x_0, 1))
        self.connect((self.correctiq_correctiq_man_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.osmosdr_source_0, 0), (self.blocks_add_const_vxx_1_0, 0))
        self.connect((self.osmosdr_source_0, 0), (self.correctiq_correctiq_1, 0))
        self.connect((self.osmosdr_source_0, 0), (self.correctiq_correctiq_auto_0, 0))
        self.connect((self.osmosdr_source_0, 0), (self.correctiq_correctiq_man_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "dcoffset_test")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.center_freq, self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_real(self):
        return self.real

    def set_real(self, real):
        self.real = real
        self.blocks_add_const_vxx_1_0.set_k(self.real + 1j * self.imag)
        self.correctiq_correctiq_man_0.set_real(self.real)

    def get_imag(self):
        return self.imag

    def set_imag(self, imag):
        self.imag = imag
        self.blocks_add_const_vxx_1_0.set_k(self.real + 1j * self.imag)
        self.correctiq_correctiq_man_0.set_imag(self.imag)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.correctiq_correctiq_auto_0.set_gain(self.gain)
        self.osmosdr_source_0.set_gain(self.gain, 0)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        Qt.QMetaObject.invokeMethod(self._center_freq_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.center_freq)))
        self.correctiq_correctiq_auto_0.set_freq(self.center_freq)
        self.osmosdr_source_0.set_center_freq(self.center_freq, 0)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.center_freq, self.samp_rate)

    def get_amp_on(self):
        return self.amp_on

    def set_amp_on(self, amp_on):
        self.amp_on = amp_on





def main(top_block_cls=dcoffset_test, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()

    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()

if __name__ == '__main__':
    main()

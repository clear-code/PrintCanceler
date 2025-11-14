/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Copyright (c) 2025 ClearCode Inc.
*/
using System;

namespace PrintCanceler
{
    internal class RuntimeContext
    {
        private readonly object lockObject = new object();

        internal Logger Logger { get; }

        internal Config Config { get; }

        internal RuntimeContext(RunTimeMode mode)
        {
            FinishTime = DateTime.Now.AddMinutes(2);
            Logger = new Logger(mode);
            Config = ConfigLoader.LoadConfig();
        }

        internal DateTime FinishTime
        {
            get
            {
                lock (lockObject)
                {
                    return _finishTime;
                }
            }
            set
            {
                lock (lockObject)
                {
                    _finishTime = value;
                }
            }
        }
        private DateTime _finishTime = DateTime.MinValue;

        internal bool IsEndTime
        {
            get
            {
                lock (lockObject)
                {
                    return _finishTime <= DateTime.Now;
                }
            }
        }
    }
}

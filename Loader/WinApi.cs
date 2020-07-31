using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Loader
{
    public class WinApi
    {
        [DllImport("Kernel32", SetLastError = true)]
        public static extern IntPtr OpenProcess(uint dwDesiredAccess, bool bInheritHandle, uint dwProcessId);

        [DllImport("Kernel32", SetLastError = true)]
        public static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("Kernel32", SetLastError = true)]
        public static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, [MarshalAs(UnmanagedType.AsAny)] object lpBuffer, uint nSize, ref uint lpNumberOfBytesWritten);
        [DllImport("Kernel32", SetLastError = true)]
        public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out uint lpNumberOfBytesRead);

        [DllImport("Kernel32", SetLastError = true)]
        public static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, ref uint lpThreadId);

        [DllImport("Kernel32", SetLastError = true)]
        public static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

        [DllImport("Kernel32", SetLastError = true)]
        public static extern bool CloseHandle(IntPtr hObject);

        #region DLL Injection
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr GetModuleHandleA(string lpModuleName);

        [DllImport("kernel32", SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
        #endregion DLL Injection
        public enum ProcessAccessRights
        {
            All = 0x001F0FFF,
            Terminate = 0x00000001,
            CreateThread = 0x00000002,
            VirtualMemoryOperation = 0x00000008,
            VirtualMemoryRead = 0x00000010,
            VirtualMemoryWrite = 0x00000020,
            DuplicateHandle = 0x00000040,
            CreateProcess = 0x000000080,
            SetQuota = 0x00000100,
            SetInformation = 0x00000200,
            QueryInformation = 0x00000400,
            QueryLimitedInformation = 0x00001000,
            Synchronize = 0x00100000
        }

        public enum MemAllocation
        {
            MEM_COMMIT = 0x00001000,
            MEM_RESERVE = 0x00002000,
            MEM_RESET = 0x00080000,
            MEM_RESET_UNDO = 0x1000000,
            SecCommit = 0x08000000
        }

        public enum MemProtect
        {
            PAGE_EXECUTE = 0x10,
            PAGE_EXECUTE_READ = 0x20,
            PAGE_EXECUTE_READWRITE = 0x40,
            PAGE_EXECUTE_WRITECOPY = 0x80,
            PAGE_NOACCESS = 0x01,
            PAGE_READONLY = 0x02,
            PAGE_READWRITE = 0x04,
            PAGE_WRITECOPY = 0x08,
            PAGE_TARGETS_INVALID = 0x40000000,
            PAGE_TARGETS_NO_UPDATE = 0x40000000,
        }
        public static void Inject(int pid, byte[] buf)
        {
            try
            {

                uint lpNumberOfBytesWritten = 0;
                uint lpThreadId = 0;
                Debug.WriteLine($"[+] 获取进程id的句柄 {pid}.");
                IntPtr pHandle = WinApi.OpenProcess((uint)WinApi.ProcessAccessRights.All, false, (uint)pid);
                IntPtr Current = WinApi.OpenProcess((uint)WinApi.ProcessAccessRights.All, false, (uint)Process.GetCurrentProcess().Id);
                Debug.WriteLine($"[+] 句柄 {pHandle} 为进程id打开{pid}.");
                IntPtr loadLibraryAddr = WinApi.GetProcAddress(WinApi.GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
                IntPtr ldrinitializethunk = WinApi.GetProcAddress(WinApi.GetModuleHandleA("ntdll.dll"), "LdrInitializeThunk");
                Debug.WriteLine($"[+] {loadLibraryAddr} 是LoadLibraryA导出函数的地址吗.");
                Debug.WriteLine($"[+] 为DLL分配内存");
                IntPtr rMemAddress = WinApi.VirtualAllocEx(pHandle, IntPtr.Zero, (uint)buf.Length, (uint)WinApi.MemAllocation.MEM_RESERVE | (uint)WinApi.MemAllocation.MEM_COMMIT, (uint)WinApi.MemProtect.PAGE_EXECUTE_READWRITE);
                Debug.WriteLine($"[+] 用于注入DLL的内存分配在 0x{rMemAddress}.");
                Debug.WriteLine($"[+] 在分配的内存位置写入DLL。");

                if (WriteProcessMemory(pHandle, rMemAddress, buf, (uint)buf.Length, ref lpNumberOfBytesWritten))
                {
                    Debug.WriteLine($"[+] DLL写入目标进程内存");
                    Debug.WriteLine($"[+] 创建远程线程来执行DLL.");
                    byte[] Temp = new byte[10];
                    if (ReadProcessMemory(Current, ldrinitializethunk, Temp, 10, out lpNumberOfBytesWritten))
                    {
                        if (WriteProcessMemory(pHandle, ldrinitializethunk, Temp, 10, ref lpNumberOfBytesWritten))
                        {
                            Debug.WriteLine("[+] 修复Hook成功!!! .");
                        }
                    }
                    IntPtr hRemoteThread = WinApi.CreateRemoteThread(pHandle, IntPtr.Zero, 0, loadLibraryAddr, rMemAddress, 0, ref lpThreadId);
                    bool hCreateRemoteThreadClose = WinApi.CloseHandle(hRemoteThread);
                    Debug.WriteLine($"[+] 成功地将DLL注入到进程id的内存中 {pid}.");
                }
                else
                {
                    Debug.WriteLine($"[+] 未能将DLL写入的内存中 进程id {pid}.");
                }
                //WaitForSingleObject(hRemoteThread, 0xFFFFFFFF);
                bool hOpenProcessClose = WinApi.CloseHandle(pHandle);
                WinApi.CloseHandle(Current);
            }
            catch (Exception)
            {

            }
        }


        public static string GetMD5HashFromFile(string filename)
        {
            try
            {
                FileStream file = new FileStream(filename, FileMode.Open);
                System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
                byte[] retVal = md5.ComputeHash(file);
                file.Close();

                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < retVal.Length; i++)
                {
                    sb.Append(retVal[i].ToString("x2"));


                }

                return sb.ToString();



            }
            catch (Exception)
            {

                return null;

            }
        }
    }
}
const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib_dep = b.dependency("raylib", .{ .target = target, .optimize = optimize });

    const exe = b.addExecutable(.{
        .name = "chess-company",
        .target = target,
        .optimize = optimize,
    });

    exe.addIncludePath(.{ .path = "src" });
    exe.addIncludePath(.{ .path = "lib/raylib/src" });

    const raylib = raylib_dep.artifact("raylib");
    raylib.defineCMacro("SUPPORT_FILEFORMAT_SVG", null);
    exe.linkLibrary(raylib);

    exe.root_module.addCSourceFiles(.{
        .root = .{ .path = "src" },
        .files = &[_][]const u8{
            "chess-engine.cpp",
            "gui.cpp",
            "main.cpp",
        },
    });

    exe.linkLibCpp();
    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

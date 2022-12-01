import java.io.File

class Elf(private val inventory: List<Int>) {
    fun totalCalories() = inventory.sum()
}

fun parse(): MutableList<Elf> {
    var elf: MutableList<Int> = mutableListOf()
    val elves: MutableList<Elf> = mutableListOf()

    File("./input.txt").forEachLine { line ->
        run {
            if (line == "") {
                elves.add(Elf(elf))
                elf = mutableListOf()
            } else elf.add(line.toInt())
        }
    }
    return elves
}

fun main() {
    val elves = parse().map { elf -> elf.totalCalories() }
    val top = elves.max()
    println("Top Elf: $top")

    val top3 = elves.sortedDescending().take(3).sum()
    println("Top 3 Elves combined: $top3")
}